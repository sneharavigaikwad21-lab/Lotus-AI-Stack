#ifndef MAMBA_BLOCK_H
#define MAMBA_BLOCK_H

#include <stdint.h>
#include "rvv_1024.h"

/**
 * @brief Initializes the Mamba state vector to all zeros.
 *
 * @param h The state vector to initialize (non-NULL, must point to at least 16 int64_t elements).
 *
 * @precondition h != NULL
 * @precondition h is aligned to 8-byte boundary (guaranteed for int64_t arrays)
 */
void mamba_state_init(mamba_vector_t h);

/**
 * @brief Performs the Mamba scan operation: hk = A*h[k-1] + B*x[k].
 *
 * @param h The current state vector (h[k-1]), updated in place to h[k].
 * @param x The input vector for the current sequence element (x[k]).
 * @param A The learned state transition matrix.
 * @param B The learned input projection matrix.
 */
void mamba_scan(mamba_vector_t h, const mamba_vector_t x, const mamba_vector_t A, const mamba_vector_t B);

/**
 * @brief Applies a learnable forget gate to the state vector.
 *
 * @param h The state vector to be gated.
 * @param forget_gate The learnable forget gate vector.
 */
void mamba_forget_gate(mamba_vector_t h, const mamba_vector_t forget_gate);

/**
 * @brief Projects the final state vector to the output.
 *
 * @param y The output vector.
 * @param h The final state vector.
 * @param C The learned output projection matrix.
 */
void mamba_output_proj(mamba_vector_t y, const mamba_vector_t h, const mamba_vector_t C);

#endif // MAMBA_BLOCK_H
