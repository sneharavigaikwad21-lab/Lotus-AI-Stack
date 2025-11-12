#ifndef AGNI_SCHEDULER_H
#define AGNI_SCHEDULER_H

#include <stdint.h>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include "config.h"

enum JobStatus {
    STATUS_QUEUED,
    STATUS_RUNNING,
    STATUS_COMPLETE,
    STATUS_ERROR
};

struct Job {
    uint32_t job_id;
    std::string weapon_id;
    std::string prompt;
    int priority;
    JobStatus status;
    std::string result;
    char error_message[256];
};

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    uint32_t submit_job(const std::string& weapon_id,
                        const std::string& prompt,
                        int priority = 0);

    Job* get_job(uint32_t job_id);
    JobStatus poll_job(uint32_t job_id);

    void start();
    void stop();

private:
    std::queue<Job> job_queue;
    std::vector<Job> job_history;
    std::vector<std::thread> workers;

    std::mutex queue_mutex;
    std::mutex history_mutex;
    std::condition_variable job_available;

    volatile bool running;
    uint32_t next_job_id;

    void worker_thread();
    void process_job(Job& job);
};

#endif // AGNI_SCHEDULER_H
