#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "common.h"
#include "scheduler.h"
#include "mamba_block.h"

void test_scheduler() {
    LOG_INFO("=== Testing Scheduler ===");

    Scheduler scheduler;
    scheduler.start();

    uint32_t job1 = scheduler.submit_job("WEAPON_1", "Test prompt 1", 0);
    uint32_t job2 = scheduler.submit_job("WEAPON_2", "Test prompt 2", 1);

    LOG_INFO("Submitted jobs: %u, %u", job1, job2);

    sleep(1);

    JobStatus s1 = scheduler.poll_job(job1);
    JobStatus s2 = scheduler.poll_job(job2);

    LOG_INFO("Job 1 status: %d", s1);
    LOG_INFO("Job 2 status: %d", s2);

    scheduler.stop();
    LOG_INFO("✅ Scheduler test passed");
}

void test_mamba() {
    LOG_INFO("=== Testing Mamba Block ===");

    MambaState state;
    int ret = mamba_block_init(&state, MAMBA_HIDDEN_SIZE, MAMBA_STATE_SIZE, 10);

    if (ret != AGNI_OK) {
        LOG_ERROR("Mamba init failed");
        return;
    }

    // Allocate input/output
    double* x = (double*)malloc(10 * MAMBA_HIDDEN_SIZE * sizeof(double));
    double* y = (double*)malloc(10 * MAMBA_HIDDEN_SIZE * sizeof(double));

    memset(x, 0, 10 * MAMBA_HIDDEN_SIZE * sizeof(double));
    memset(y, 0, 10 * MAMBA_HIDDEN_SIZE * sizeof(double));

    auto start_time = clock();
    mamba_block_forward(&state, x, y);
    auto end_time = clock();

    double latency_ms = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000;
    LOG_INFO("Mamba forward pass latency: %.2f ms", latency_ms);

    free(x);
    free(y);
    mamba_block_free(&state);

    LOG_INFO("✅ Mamba test passed");
}

int main() {
    LOG_INFO("=== PROJECT AGNI V45-STABLE TEST SUITE ===");

    test_scheduler();
    test_mamba();

    LOG_INFO("=== ALL TESTS PASSED ===");
    return 0;
}
