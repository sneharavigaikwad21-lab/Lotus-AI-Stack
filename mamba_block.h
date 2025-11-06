#ifndef MAMBA_BLOCK_H_R6
#define MAMBA_BLOCK_H_R6

#include "Agni_Vector/rvv_1024.h"    // Our RVV foundation
#include "scheduler.h"              // Core Command foundation

struct mamba_block {
  double* A; // Ptr to A matrix/vector (d_state)
  double* B; // Ptr to B matrix (d_state, d_model)
  double* C; // Ptr to C matrix (d_model, d_state)
  double* h; // Ptr to persistent state h (d_state)
  double* u; // <-- (REVISION 6 FIX) Ptr to temp buffer (d_state)

  int d_model; // Full dimension of model
  int d_state; // Full dimension of state
};

/**
 * @brief Initializes the Mamba block struct with pointers to "Key L2" SRAM.
 * @param pU Pointer to a temporary buffer of size (d_state)
 * This buffer MUST be in "Key L2" SRAM (0x3...).
 */
void mamba_block_init(struct mamba_block* block,
                      double* pA, double* pB, double* pC,
                      double* pState, double* pU, // <-- (REVISION 6 FIX)
                      int d_model, int d_state);

/**
 * @brief Executes the *full* Mamba block "strip-mine" loop using RVV 1.0.
 *
 * @warning **MEMORY FENCE (TRM v2.0, Sec 5.0):**
 * The CALLER (Scheduler) *MUST* call agni_memory_fence() *before*
 * calling this function if 'x_input' is from a 'lotus_noc'
 * transfer, to prevent data corruption.
 *
 * @param block Pointer to the initialized block.
 * @param x_input Pointer to the *full* input vector (d_model elements).
 * @param y_output Pointer to the *full* output vector (d_model elements).
 */
void mamba_block_execute(struct mamba_block* block,
                         double* x_input,
                         double* y_output);

#endif // MAMBA_BLOCK_H_R6
