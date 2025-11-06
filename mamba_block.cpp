#include "mamba_block.h"

void mamba_block_init(struct mamba_block* block,
                      double* pA, double* pB, double* pC,
                      double* pState, double* pU, // <-- (REVISION 6 FIX)
                      int d_model, int d_state) {
  block->A = pA;
  block->B = pB;
  block->C = pC;
  block->h = pState;
  block->u = pU; // <-- (REVISION 6 FIX)
  block->d_model = d_model;
  block->d_state = d_state;
}

void mamba_block_execute(struct mamba_block* block,
                         double* x_input,
                         double* y_output) {

  size_t vl;
  size_t d_model = block->d_model;
  size_t d_state = block->d_state;

  // "Golden Path" (REVISION 6 FIX): Get temp buffer from struct.
  // This buffer is already in "Key L2" SRAM. NO malloc().
  double* u = block->u;

  // ---
  // STEP 1: Project Input (u = B @ x)
  // (This logic is "Golden Knowledge" from R5. DO NOT CHANGE IT.)
  // ---
  for (size_t i = 0; i < d_state; i++) {
    size_t vl_gemv;
    vfloat64m1_t acc_vec = __riscv_vfmv_v_f_f64m1(0.0, 1);
    for (size_t j = 0; j < d_model; j += vl_gemv) {
      vl_gemv = agni_set_vl_e64m8(d_model - j);
      const double *b_ptr = block->B + i * d_model + j;
      const double *x_ptr = x_input + j;
      vfloat64m8_t b_vec = agni_vload_e64m8(b_ptr, vl_gemv);
      vfloat64m8_t x_vec = agni_vload_e64m8(x_ptr, vl_gemv);
      vfloat64m8_t prod_vec = __riscv_vfmul_vv_f64m8(b_vec, x_vec, vl_gemv);
      acc_vec = __riscv_vfredusum_vs_f64m8_f64m1(prod_vec, acc_vec, vl_gemv);
    }
    u[i] = __riscv_vfmv_f_s_f64m1_f64(acc_vec);
  }

  // ---
  // STEP 2: Update State (h = A*h + u)
  // (This logic is "Golden Knowledge" from R5. DO NOT CHANGE IT.)
  // ---
  for (size_t l = 0; l < d_state; l += vl) {
    vl = agni_set_vl_e64m8(d_state - l);
    vfloat64m8_t h_vec = agni_vload_e64m8(block->h + l, vl);
    vfloat64m8_t A_vec = agni_vload_e64m8(block->A + l, vl);
    vfloat64m8_t u_vec = agni_vload_e64m8(u + l, vl); // Load from block->u
    vfloat64m8_t Ah = __riscv_vfmul_vv_f64m8(A_vec, h_vec, vl);
    h_vec = __riscv_vfadd_vv_f64m8(Ah, u_vec, vl);
    agni_vstore_e64m8(block->h + l, h_vec, vl);
  }

  // ---
  // STEP 3: Project Output (y = C @ h)
  // (This logic is "Golden Knowledge" from R5. DO NOT CHANGE IT.)
  // ---
  for (size_t i = 0; i < d_model; i++) {
    size_t vl_gemv;
    vfloat64m1_t acc_vec = __riscv_vfmv_v_f_f64m1(0.0, 1);
    for (size_t j = 0; j < d_state; j += vl_gemv) {
      vl_gemv = agni_set_vl_e64m8(d_state - j);
      const double *c_ptr = block->C + i * d_state + j;
      const double *h_ptr = block->h + j;
      vfloat64m8_t c_vec = agni_vload_e64m8(c_ptr, vl_gemv);
      vfloat64m8_t h_vec = agni_vload_e64m8(h_ptr, vl_gemv);
      vfloat64m8_t prod_vec = __riscv_vfmul_vv_f64m8(c_vec, h_vec, vl_gemv);
      acc_vec = __riscv_vfredusum_vs_f64m8_f64m1(prod_vec, acc_vec, vl_gemv);
    }
    y_output[i] = __riscv_vfmv_f_s_f64m1_f64(acc_vec);
  }

  // (The 'free(u)' call is GONE. This is the "Zero-Bug" fix.)
}
