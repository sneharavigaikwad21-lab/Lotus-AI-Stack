#ifndef SCHEDULER_C_WRAPPER_H
#define SCHEDULER_C_WRAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    JOB_STATE_QUEUED,
    JOB_STATE_RUNNING,
    JOB_STATE_COMPLETE,
    JOB_STATE_ERROR
} JobState;

typedef struct {
    uint32_t job_id;
    char* prompt;
    int priority;
    JobState state;
} Job_t;

// Scheduler interface
void* scheduler_init(void);
int scheduler_submit(void* sched, Job_t* job);
int scheduler_poll(void* sched, uint32_t job_id, JobState* state);
void scheduler_cleanup(void* sched);

#ifdef __cplusplus
}
#endif

#endif // SCHEDULER_C_WRAPPER_H
