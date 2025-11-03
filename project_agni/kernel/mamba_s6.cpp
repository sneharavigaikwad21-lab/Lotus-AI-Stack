#include "rvv_1024.h"

/**
 * @brief Mamba S6 state-space model kernel.
 *
 * This function implements the Mamba S6 state-space model, as described in
 * the research paper "OSMamba: Omnidirectional Spectral Mamba with
 * Dual-Domain Prior Generator for Exposure Correction".
 *
 * @param A The state transition matrix.
 * @param B The input matrix.
 * @param C The output matrix.
 * @param x The input vector.
 * @param h The state vector.
 * @param y The output vector.
 */
void mamba_s6(v1024_t A, v1024_t B, v1024_t C, v1024_t x, v1024_t* h, v1024_t* y) {
  // Load the current state vector
  v1024_t h_current = rvv_1024_load(h);

  // hk = Ahk−1 + Bxk
  v1024_t h_new = rvv_1024_add(rvv_1024_mul(A, h_current), rvv_1024_mul(B, x));

  // yk = Chk
  v1024_t y_new = rvv_1024_mul(C, h_new);

  // Store the new state and output vectors
  rvv_1024_store(h, h_new);
  rvv_1024_store(y, y_new);
}
