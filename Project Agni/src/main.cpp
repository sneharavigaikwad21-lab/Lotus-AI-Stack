#include "scheduler.h"

/**
 * @brief Main entry point for the "Core Command" executable.
 *
 * This function initializes the scheduler and starts its infinite execution
 * loop. It represents the sole responsibility of the "Foreman" CPU core.
 *
 * @return int - Standard exit code (will not be reached).
 */
int main() {
    // Initialize the scheduler, task queues, and hardware interrupts.
    scheduler_init();

    // Start the main execution loop. This function never returns.
    scheduler_run_loop();

    // This line is unreachable but satisfies the compiler's requirement
    // for a return value from main.
    return 0;
}
