#include "mamba_block.h"
#include "rvv_1024.h"

/**
 * @brief Initializes the Mamba state vector to all zeros using RVV intrinsics.
 * @pseudo-assembly
 *   vsetvli t0, a0, e64, m1, ta, ma # Set vector length
 *   vmv.v.i v0, 0                  # Move immediate 0 to vector register
 *   vse64.v v0, (a0)               # Store vector register to memory
 *   fence                        # V8 Brute-Force Proof
 * @end-pseudo-assembly
 */
void mamba_state_init(mamba_vector_t h) {
    size_t vl;
    for (size_t i = 0; i < NUM_FEATURES; i += vl) {
        vl = vsetvl_e64m1(NUM_FEATURES - i);
        vint64m1_t v_zero = vmv_v_x_i64m1(0, vl);
        vse64_v_i64m1(h + i, v_zero, vl);
        asm volatile("fence");
    }
}

/**
 * @brief Performs the Mamba scan operation: hk = A*h[k-1] + B*x[k].
 * @pseudo-assembly
 *   # This represents one chunk of the loop
 *   vsetvli t0, a0, e64, m1, ta, ma
 *   vle64.v v0, (a2)      # Load A chunk
 *   fence
 *   vle64.v v1, (a0)      # Load h chunk
 *   fence
 *   vmul.vv v2, v0, v1    # A * h
 *   vle64.v v0, (a3)      # Load B chunk
 *   fence
 *   vle64.v v1, (a1)      # Load x chunk
 *   fence
 *   vmul.vv v3, v0, v1    # B * x
 *   vadd.vv v2, v2, v3    # (A*h) + (B*x)
 *   vse64.v v2, (a0)      # Store result back to h
 *   fence
 * @end-pseudo-assembly
 */
void mamba_scan(mamba_vector_t h, const mamba_vector_t x, const mamba_vector_t A, const mamba_vector_t B) {
    mamba_vector_t temp1, temp2;
    rvv_1024_mul(temp1, A, h);
    rvv_1024_mul(temp2, B, x);
    rvv_1024_add(h, temp1, temp2);
}

/**
 * @brief Applies a learnable forget gate to the state vector.
 * @pseudo-assembly
 *   vsetvli t0, a0, e64, m1, ta, ma
 *   vle64.v v0, (a0)      # Load h chunk
 *   fence
 *   vle64.v v1, (a1)      # Load forget_gate chunk
 *   fence
 *   vmul.vv v0, v0, v1    # h * forget_gate
 *   vse64.v v0, (a0)      # Store result back to h
 *   fence
 * @end-pseudo-assembly
 */
void mamba_forget_gate(mamba_vector_t h, const mamba_vector_t forget_gate) {
    rvv_1024_mul(h, h, forget_gate);
}

/**
 * @brief Projects the final state vector to the output.
 * @pseudo-assembly
 *   vsetvli t0, a0, e64, m1, ta, ma
 *   vle64.v v0, (a1)      # Load h chunk
 *   fence
 *   vle64.v v1, (a2)      # Load C chunk
 *   fence
 *   vmul.vv v0, v0, v1    # h * C
 *   vse64.v v0, (a0)      # Store result to y
 *   fence
 * @end-pseudo-assembly
 */
void mamba_output_proj(mamba_vector_t y, const mamba_vector_t h, const mamba_vector_t C) {
    rvv_1024_mul(y, h, C);
}
