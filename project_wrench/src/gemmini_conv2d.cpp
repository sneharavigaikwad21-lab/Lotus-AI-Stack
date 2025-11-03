#include "gemmini_conv2d.h"
#include "gemmini_rocc.h"

// Global variable to hold the registered callback function
static gemmini_callback_t g_on_complete_callback = NULL;

// Define scratchpad size and tile size based on the new spec
#define GEMMINI_SCRATCHPAD_SIZE (256 * 1024) // 256 KB
#define GEMMINI_TILE_SIZE 64

/**
 * @brief Computes a dynamic scratchpad address for a given tile index.
 * This prevents different tiles from overwriting each other in the scratchpad.
 */
uint64_t compute_scratchpad_addr(int tile_index) {
    return (tile_index * GEMMINI_TILE_SIZE * GEMMINI_TILE_SIZE) % GEMMINI_SCRATCHPAD_SIZE;
}

void gemmini_conv2d_batch_async(
    const int8_t* input,
    const int8_t* weights,
    int8_t* output,
    int H,
    int W,
    int C_in,
    int C_out,
    gemmini_callback_t on_complete
) {
    // This is a simplified control loop for a single tile.

    // 1. Register the callback function
    g_on_complete_callback = on_complete;

    // 2. Compute dynamic scratchpad addresses
    uint64_t sp_addr_in = compute_scratchpad_addr(0);
    uint64_t sp_addr_w = compute_scratchpad_addr(1);
    uint64_t sp_addr_out = compute_scratchpad_addr(2);

    // 3. Stage input and weight tiles
    gemmini_write_tile((uint64_t)input, sp_addr_in);
    gemmini_write_tile((uint64_t)weights, sp_addr_w);

    // 4. Issue asynchronous GEMM command
    gemmini_execute_gemm(sp_addr_in, sp_addr_w, sp_addr_out);

    // In a real system, the final 'read tile' operation would be chained
    // by the hardware or the interrupt handler. For this simulation,
    // we assume it's part of the async execution.

    // 5. Return immediately. The hardware is now "running".
}

/**
 * @brief Simulates the hardware interrupt service routine (ISR).
 * When the Gemmini co-processor finishes, the CPU would jump here.
 */
void gemmini_isr_simulation() {
    if (g_on_complete_callback != NULL) {
        g_on_complete_callback();
    }
}
