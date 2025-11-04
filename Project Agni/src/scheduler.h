#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "task_queue.h"
#include <stdatomic.h>

// The standard function pointer for all executable tasks
typedef void (*task_function_t)(void* args);

typedef struct Task {
    struct Task* next_in_queue;
    task_function_t function;
    void* arguments;
    atomic_int dependency_count;
} Task;

typedef void HW_NoC_Descriptor; // Opaque type

typedef struct Scheduler_NoC_Job {
    struct Scheduler_NoC_Job* next_in_queue;
    HW_NoC_Descriptor* hw_descriptor;
    Task** dependent_tasks; // NULL-terminated array of pointers
} Scheduler_NoC_Job;

// Foundation Queues (from V21.1 'task_queue.h')
extern TaskQueue* g_ready_queue;
extern TaskQueue* g_noc_job_queue;

// Current NoC State (MUST be volatile)
extern volatile Scheduler_NoC_Job* g_current_noc_job;

// "Golden Knowledge" (V26) Register Pointers
extern volatile uint64_t* NOC_SEND_CMD_REG;
extern volatile uint64_t* NOC_STATUS_REG;
extern volatile uint64_t* NOC_IRQ_ENABLE_REG;

void scheduler_init();
void scheduler_run_loop();
void scheduler_add_task(Task* task);
void scheduler_add_noc_job(Scheduler_NoC_Job* noc_job);
void NOC_IRQ_Handler();

#endif // SCHEDULER_H_
