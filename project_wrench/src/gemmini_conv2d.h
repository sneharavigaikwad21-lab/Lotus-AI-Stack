#ifndef GEMMINI_CONV2D_H
#define GEMMINI_CONV2D_H

#include <stdint.h>
#include "gemmini_rocc.h"

// Define a callback function pointer type for asynchronous operations
typedef void (*gemmini_callback_t)(void);

/**
 * @brief Performs a batched 2D convolution asynchronously using Gemmini.
 *
 * This function issues all necessary RoCC commands to perform the convolution
 * and then returns immediately. It registers a callback function to be executed
 * by a simulated interrupt handler upon completion.
 *
 * @param input Pointer to the input feature maps (must be 64-byte aligned).
 * @param weights Pointer to the convolution filter weights (must be 64-byte aligned).
 * @param output Pointer to the output feature maps (must be 64-byte aligned).
 * @param H Height of the input feature maps.
 * @param W Width of the input feature maps.
 * @param C_in Number of input channels.
 * @param C_out Number of output channels.
 * @param on_complete The callback function to execute when the operation is finished.
 *
 * @precondition All pointers must be non-NULL and 64-byte aligned.
 */
void gemmini_conv2d_batch_async(
    const int8_t* input,
    const int8_t* weights,
    int8_t* output,
    int H,
    int W,
    int C_in,
    int C_out,
    gemmini_callback_t on_complete
);

/**
 * @brief Simulates a Gemmini interrupt handler.
 * In a real system, this would be invoked by a hardware interrupt when the
 * co-processor is no longer busy. It executes the registered callback.
 */
void gemmini_isr_simulation();

#endif // GEMMINI_CONV2D_H
