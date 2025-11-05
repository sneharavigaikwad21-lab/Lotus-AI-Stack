#include "scheduler.h"
#include <cstdint> // For uint64_t

// --- Global Variable Definitions ---
// These variables are declared 'extern' in scheduler.h and defined here.
TaskQueue* g_ready_queue;
TaskQueue* g_noc_job_queue;
volatile Scheduler_NoC_Job* g_current_noc_job;

// "Golden Knowledge" (V26) Register Pointers
// These are initialized in scheduler_init().
volatile uint64_t* NOC_SEND_CMD_REG;
volatile uint64_t* NOC_STATUS_REG;
volatile uint64_t* NOC_IRQ_ENABLE_REG;

// --- Assumed External Hardware-Specific Functions ---
// These would be provided by the board support package (BSP) or HAL.
// We provide dummy declarations here to allow the scheduler to compile.
#define NOC_VECTOR 1 // Example interrupt vector number for the NoC
extern "C" void register_interrupt_handler(int vector, void (*handler)());
extern "C" void acknowledge_irq(int vector);

// --- Static Function Prototype ---
// This is an internal helper function for the scheduler.
static void scheduler_issue_next_noc_job();

// --- Public Function Implementations ---

void scheduler_init() {
    // 1. Allocate memory for the queues
    g_ready_queue = new TaskQueue;
    g_noc_job_queue = new TaskQueue;

    // Initialize the queues to a clean, empty state.
    task_queue_init(g_ready_queue);
    task_queue_init(g_noc_job_queue);

    // Ensure the NoC job state is initially null.
    g_current_noc_job = NULL;

    // Initialize "Golden Knowledge" register pointers to their V26 addresses.
    NOC_SEND_CMD_REG   = (volatile uint64_t*)0x40000000;
    NOC_STATUS_REG     = (volatile uint64_t*)0x40000008;
    NOC_IRQ_ENABLE_REG = (volatile uint64_t*)0x4000000C;

    // Enable the "DONE" interrupt for the NoC hardware.
    *NOC_IRQ_ENABLE_REG = 0x1;

    // Register our ISR as the handler for the NoC's interrupt vector.
    register_interrupt_handler(NOC_VECTOR, NOC_IRQ_Handler);
}

void scheduler_run_loop() {
    // This is the main "Foreman" (CVA6S+) CPU loop. It runs forever.
    while (1) {
        // Pop a task from the ready queue. This is a non-blocking operation.
        Task* task = (Task*)task_queue_pop(g_ready_queue);

        if (task != NULL) {
            // If a ready task was found, execute its function pointer.
            task->function(task->arguments);
        } else {
            // If the ready queue is empty, the CPU is idle.
            // On a real system, a `wfi` (Wait For Interrupt) instruction
            // could be placed here to save power. For now, we simply loop.
        }
        // This loop is the "Software DMA" (Solution 2). By constantly
        // checking for and executing ready CPU-bound tasks, this loop
        // runs productive work on the "Foreman" CPU *simultaneously*
        // while the "NoC Interface" hardware is busy with its 40ms copy.
        // This perfectly hides the data transfer latency, as mandated.
    }
}

void scheduler_add_task(Task* task) {
    // Atomically check the dependency counter. If it's zero, the task is
    // ready to run immediately.
    if (atomic_load(&task->dependency_count) == 0) {
        task_queue_push(g_ready_queue, (void*)task);
    }
    // If the dependency count is > 0, the task is blocked. The ISR is
    // responsible for eventually decrementing the count and making it ready.
}

void scheduler_add_noc_job(Scheduler_NoC_Job* noc_job) {
    // Add the NoC job to the queue of pending transfers.
    task_queue_push(g_noc_job_queue, (void*)noc_job);

    // Attempt to start the NoC transfer immediately if the hardware is idle.
    scheduler_issue_next_noc_job();
}

void NOC_IRQ_Handler() {
    // If g_current_noc_job is NULL, this is a spurious interrupt.
    // Acknowledge it and return immediately to avoid processing invalid data.
    if (g_current_noc_job == NULL) {
        acknowledge_irq(NOC_VECTOR);
        return;
    }

    // Get the job that just finished.
    Scheduler_NoC_Job* finished_job = (Scheduler_NoC_Job*)g_current_noc_job;

    // IMPORTANT: Clear the global state *before* unblocking tasks. This signals
    // that the NoC hardware is now available.
    g_current_noc_job = NULL;

    // Unblock all tasks that were dependent on this NoC transfer.
    Task** tasks = finished_job->dependent_tasks;
    if (tasks != NULL) {
        while (*tasks != NULL) {
            Task* task_to_unblock = *tasks;

            // Decrement the dependency counter atomically.
            if (atomic_fetch_sub(&task_to_unblock->dependency_count, 1) == 1) {
                // If the count becomes zero, the task is now ready to run.
                task_queue_push(g_ready_queue, (void*)task_to_unblock);
            }
            tasks++; // Move to the next pointer in the NULL-terminated list.
        }
    }

    // Keep the hardware pipeline full. Immediately try to issue the next
    // pending NoC job, if one exists.
    scheduler_issue_next_noc_job();

    // Acknowledge the interrupt to the hardware interrupt controller (e.g., PLIC).
    acknowledge_irq(NOC_VECTOR);
}

// --- Static Function Implementation ---

static void scheduler_issue_next_noc_job() {
    // This function is called from the ISR and from scheduler_add_noc_job,
    // so it MUST be protected by a spinlock to prevent race conditions where
    // two callers try to issue a job simultaneously.
    spinlock_lock(&g_noc_job_queue->lock);

    // Check if the hardware is idle AND if we aren't already tracking a job.
    // This double-check prevents issuing a new job while an old one is in-flight
    // but the ISR hasn't fired yet.
    if (*NOC_STATUS_REG == 0x0 && g_current_noc_job == NULL) {
        // Pop a job from the queue. Note: We are already holding the lock,
        // so we perform the pop logic manually instead of calling task_queue_pop()
        // to avoid a deadlock (double-locking).
        // Get the head node.
        void* node = g_noc_job_queue->head;

        if (node != NULL) {
            // Cast to GenericQueueNode to safely access the 'next' pointer.
            GenericQueueNode* n = (GenericQueueNode*)node;

            // Move head to the next node.
            g_noc_job_queue->head = n->next;

            // If the queue is now empty, update the tail as well.
            if (g_noc_job_queue->head == NULL) {
                g_noc_job_queue->tail = NULL;
            }

            // Now it's safe to cast to the specific job type.
            Scheduler_NoC_Job* job = (Scheduler_NoC_Job*)node;

            // Set this as the globally tracked "current" job.
            g_current_noc_job = job;

            // Fire the "Software DMA": Write the hardware descriptor pointer
            // to the NoC command register to start the 40ms transfer.
            // This is a non-blocking, fire-and-forget write.
            *NOC_SEND_CMD_REG = (uint64_t)job->hw_descriptor;
        }
    }

    // Release the lock.
    spinlock_unlock(&g_noc_job_queue->lock);
}
