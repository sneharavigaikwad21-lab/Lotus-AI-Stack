#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

#include <stdatomic.h> // For atomic_flag
#include <stddef.h>    // For NULL

/**
 * @brief A spinlock built from C atomics.
 */
typedef atomic_flag Spinlock;

/**
 * @brief The core intrusive FIFO queue.
 * It does NOT allocate memory; it manages nodes given to it.
 */
typedef struct TaskQueue {
    Spinlock lock;
    void* head;
    void* tail;
} TaskQueue;

/**
 * @brief A helper struct for casting.
 * This allows the queue to manage any struct that has
 * a 'void* next' pointer as its *first* member.
 */
typedef struct GenericQueueNode {
    void* next;
} GenericQueueNode;

/**
 * @brief Initializes a spinlock to the "unlocked" state.
 */
static inline void spinlock_init(Spinlock* lock) {
    atomic_flag_clear_explicit(lock, memory_order_release);
}

/**
 * @brief Acquires the lock, spinning until it is successful.
 */
static inline void spinlock_lock(Spinlock* lock) {
    while (atomic_flag_test_and_set_explicit(lock, memory_order_acquire)) {
        // Spin-wait (hardware-specific pause can be added later)
    }
}

/**
 * @brief Releases the lock.
 */
static inline void spinlock_unlock(Spinlock* lock) {
    atomic_flag_clear_explicit(lock, memory_order_release);
}

/**
 * @brief Initializes an empty TaskQueue.
 */
static inline void task_queue_init(TaskQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    spinlock_init(&queue->lock);
}

/**
 * @brief Pushes a new node onto the *tail* of the queue (FIFO).
 * @param queue The queue to modify.
 * @param node A pointer to the node to add. The node *must* have a
 * 'void* next' pointer as its first member.
 */
static inline void task_queue_push(TaskQueue* queue, void* node) {
    GenericQueueNode* n = (GenericQueueNode*)node;
    n->next = NULL;

    spinlock_lock(&queue->lock);

    if (queue->tail == NULL) {
        // Queue was empty
        queue->head = node;
        queue->tail = node;
    } else {
        // Queue not empty, add to tail
        ((GenericQueueNode*)queue->tail)->next = node;
        queue->tail = node;
    }

    spinlock_unlock(&queue->lock);
}

/**
 * @brief Pops a node from the *head* of the queue (FIFO).
 * @param queue The queue to modify.
 * @return The head node, or NULL if the queue was empty.
 */
static inline void* task_queue_pop(TaskQueue* queue) {
    spinlock_lock(&queue->lock);

    if (queue->head == NULL) {
        // Queue is empty
        spinlock_unlock(&queue->lock);
        return NULL;
    }

    // Get the head node
    void* node = queue->head;
    GenericQueueNode* n = (GenericQueueNode*)node;

    // Move head to the next node
    queue->head = n->next;

    if (queue->head == NULL) {
        // The queue is now empty
        queue->tail = NULL;
    }

    spinlock_unlock(&queue->lock);
    return node;
}

#endif // TASK_QUEUE_H_
