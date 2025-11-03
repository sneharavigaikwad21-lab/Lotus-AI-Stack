#include "gemmini.h"
#include <stdint.h>

// Define dimensions for a sample convolution
#define BATCH_SIZE 1
#define IN_CHANNELS 3
#define IN_DIM 8
#define OUT_CHANNELS 4
#define KERNEL_DIM 3
#define OUT_DIM (IN_DIM - KERNEL_DIM + 1)

/**
 * @brief A realistic convolution implementation for the Gemmini co-processor.
 *
 * This function demonstrates a simplified weight tiling scheme. For each
 * output channel, it loads the corresponding filter weights into the Gemmini
 * scratchpad. Then, it iterates through the input image, loading input tiles,
 * executing the matrix multiplication, and storing the result back to main memory.
 *
 * NOTE: This is a high-level simulation. A real implementation would require
 * an im2col transformation and more complex scratchpad address management.
 */
void tiled_conv2d(const int8_t* input, const int8_t* weights, int8_t* output) {
    // Define base addresses in the Gemmini scratchpad
    uint64_t sp_addr_input = 0;
    uint64_t sp_addr_weights = 1024;
    uint64_t sp_addr_output = 2048;

    for (int oc = 0; oc < OUT_CHANNELS; ++oc) {
        // --- Weight Tiling ---
        // Load the filter weights for the current output channel into the scratchpad.
        const int8_t* weight_tile_src = weights + (oc * IN_CHANNELS * KERNEL_DIM * KERNEL_DIM);
        gemmini_mvin((uint64_t)weight_tile_src, sp_addr_weights);

        for (int oh = 0; oh < OUT_DIM; ++oh) {
            for (int ow = 0; ow < OUT_DIM; ++ow) {
                // --- Input Tiling ---
                // Load the corresponding input image patch (tile) into the scratchpad.
                // This simulates the im2col transformation.
                const int8_t* input_tile_src = input + (oh * IN_DIM) + ow;
                gemmini_mvin((uint64_t)input_tile_src, sp_addr_input);

                // --- Computation ---
                // Execute the matrix multiplication on the tiles in the scratchpad.
                gemmini_compute_matmul(sp_addr_input, sp_addr_weights, sp_addr_output);

                // --- Output ---
                // Move the single output pixel from the scratchpad back to main memory.
                int8_t* output_pixel_dest = output + (oc * OUT_DIM * OUT_DIM) + (oh * OUT_DIM) + ow;
                gemmini_mvout(sp_addr_output, (uint64_t)output_pixel_dest);
            }
        }
    }
}

class LotusSight {
public:
  void conv2d(const int8_t* input, const int8_t* weights, int8_t* output) {
      tiled_conv2d(input, weights, output);
  }
};
