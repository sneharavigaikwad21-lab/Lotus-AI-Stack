#ifndef RVV_1024_H
#define RVV_1024_H

#include <riscv_vector.h>
#include <stdint.h>

#define NUM_FEATURES 16

// A 1024-bit vector, represented as a memory buffer of 16, 64-bit elements
typedef int64_t mamba_vector_t[NUM_FEATURES];

// Correctly adds two 1024-bit vectors with the V8 Brute-Force Proof
static inline void rvv_1024_add(mamba_vector_t dest, const mamba_vector_t src1, const mamba_vector_t src2) {
    size_t vl;
    for (size_t i = 0; i < NUM_FEATURES; i += vl) {
        vl = vsetvl_e64m1(NUM_FEATURES - i);
        vint64m1_t v1 = vle64_v_i64m1(src1 + i, vl);
        asm volatile("fence");
        vint64m1_t v2 = vle64_v_i64m1(src2 + i, vl);
        asm volatile("fence");
        vint64m1_t v_res = vadd_vv_i64m1(v1, v2, vl);
        vse64_v_i64m1(dest + i, v_res, vl);
        asm volatile("fence");
    }
}

// Correctly multiplies two 1024-bit vectors with the V8 Brute-Force Proof
static inline void rvv_1024_mul(mamba_vector_t dest, const mamba_vector_t src1, const mamba_vector_t src2) {
    size_t vl;
    for (size_t i = 0; i < NUM_FEATURES; i += vl) {
        vl = vsetvl_e64m1(NUM_FEATURES - i);
        vint64m1_t v1 = vle64_v_i64m1(src1 + i, vl);
        asm volatile("fence");
        vint64m1_t v2 = vle64_v_i64m1(src2 + i, vl);
        asm volatile("fence");
        vint64m1_t v_res = vmul_vv_i64m1(v1, v2, vl);
        vse64_v_i64m1(dest + i, v_res, vl);
        asm volatile("fence");
    }
}

#endif // RVV_1024_H
