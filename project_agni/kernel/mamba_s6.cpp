#include "rvv_1024.h"

/**
 * @brief Mamba S6 state-space model kernel (Re-forged).
 *
 * This function implements the Mamba S6 state-space model using
 * realistic, iterated RISC-V V 1.0 intrinsics.
 *
 * @param A The state transition matrix (1024-bit).
 * @param B The input matrix (1024-bit).
 * @param C The output matrix (1024-bit).
 * @param x The input vector (1024-bit).
 * @param h The state vector (1024-bit, in/out).
 * @param y The output vector (1024-bit, out).
 */
void mamba_s6(const int64_t* A, const int64_t* B, const int64_t* C, const int64_t* x, int64_t* h, int64_t* y) {
    // Temporary arrays for intermediate calculations
    int64_t temp1[VLEN_1024_IN_64];
    int64_t temp2[VLEN_1024_IN_64];
    int64_t h_new[VLEN_1024_IN_64];

    // Load the current state vector
    int64_t h_current[VLEN_1024_IN_64];
    rvv_1024_load(h_current, h);

    // hk = Ahk−1 + Bxk
    rvv_1024_mul(temp1, A, h_current);
    rvv_1024_mul(temp2, B, x);
    rvv_1024_add(h_new, temp1, temp2);

    // yk = Chk
    rvv_1024_mul(y, C, h_new);

    // Store the new state and output vectors
    rvv_1024_store(h, h_new);
}
