#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "scheduler.h"
#include "api_gateway.h"
#include "firebase_auth.h"

int main(int, char*[]) {
    LOG_INFO("=== PROJECT AGNI STARTING ===");
    LOG_INFO("Target: RISC-V 64-bit (RV64GC)");
    LOG_INFO("Version: V45-STABLE");
    LOG_INFO("System Timestamp: 2025-11-11 09:08:19.709650");

    // Initialize components
    Scheduler scheduler;
    scheduler.start();

    APIGateway gateway(&scheduler);
    gateway.start_server(8080);

    FirebaseAuth auth;

    LOG_INFO("✅ All systems initialized");
    LOG_INFO("API Gateway listening on port 8080");

    // Submit test job
    LOG_INFO("Submitting test job...");
    uint32_t job_id = scheduler.submit_job("WEAPON_1", "Hello, Lotus AI!");
    LOG_INFO("Test job submitted with ID: %u", job_id);

    // Poll for result
    sleep(2);
    JobStatus status = scheduler.poll_job(job_id);
    LOG_INFO("Job status: %d", status);

    // Keep running for API requests
    LOG_INFO("System running. Press Ctrl+C to exit.");
    while (1) {
        sleep(1);
        // Wait for Ctrl+C to break loop
        // (signal handler catches SIGINT)
    }

    // This code is unreachable but needed for cleanup:
    scheduler.stop();
    LOG_INFO("PROJECT AGNI SHUTDOWN");

    return 0;
}
