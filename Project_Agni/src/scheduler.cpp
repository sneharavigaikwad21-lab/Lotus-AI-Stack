#include "scheduler.h"
#include "common.h"
#include <chrono>
#include <atomic>

Scheduler::Scheduler() : running(false), next_job_id(1) {
    LOG_INFO("Scheduler initialized");
}

Scheduler::~Scheduler() {
    stop();
}

uint32_t Scheduler::submit_job(const std::string& weapon_id,
                               const std::string& prompt,
                               int priority) {
    std::lock_guard<std::mutex> lock(queue_mutex);

    Job job;
    job.job_id = next_job_id++;
    job.weapon_id = weapon_id;
    job.prompt = prompt;
    job.priority = priority;
    job.status = STATUS_QUEUED;

    job_queue.push(job);
    job_available.notify_one();

    LOG_INFO("Job submitted: id=%u, weapon=%s", job.job_id, weapon_id.c_str());
    return job.job_id;
}

Job* Scheduler::get_job(uint32_t job_id) {
    std::lock_guard<std::mutex> lock(history_mutex);

    for (auto& job : job_history) {
        if (job.job_id == job_id) {
            return &job;
        }
    }
    return nullptr;
}

JobStatus Scheduler::poll_job(uint32_t job_id) {
    Job* job = get_job(job_id);
    if (!job) return STATUS_ERROR;
    return job->status;
}

void Scheduler::start() {
    if (running) return;
    running = true;

    for (int i = 0; i < MAX_WORKERS; i++) {
        workers.push_back(std::thread(&Scheduler::worker_thread, this));
    }
    LOG_INFO("Scheduler started with %d workers", MAX_WORKERS);
}

void Scheduler::stop() {
    running = false;
    job_available.notify_all();

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers.clear();
    LOG_INFO("Scheduler stopped");
}

void Scheduler::worker_thread() {
    while (running) {
        std::unique_lock<std::mutex> lock(queue_mutex);

        job_available.wait(lock, [this]() {
            return !job_queue.empty() || !running;
        });

        if (!running) break;

        if (job_queue.empty()) continue;

        Job job = job_queue.front();
        job_queue.pop();
        lock.unlock();

        job.status = STATUS_RUNNING;
        process_job(job);
        job.status = STATUS_COMPLETE;

        std::lock_guard<std::mutex> history_lock(history_mutex);
        job_history.push_back(job);

        LOG_INFO("Job completed: id=%u", job.job_id);
    }
}

void Scheduler::process_job(Job& job) {
    auto start = std::chrono::high_resolution_clock::now();

    // Route to correct weapon
    if (job.weapon_id == "WEAPON_1") {
        // Mamba Brain - text generation
        job.result = "Generated text from Mamba for: " + job.prompt;
    } else if (job.weapon_id == "WEAPON_2") {
        // Search/RAG
        job.result = "Search results for: " + job.prompt;
    } else {
        snprintf(job.error_message, sizeof(job.error_message),
                "Unknown weapon: %s", job.weapon_id.c_str());
        job.status = STATUS_ERROR;
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto latency_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start).count();

    LOG_INFO("Job %u latency: %ld ms", job.job_id, latency_ms);
}
