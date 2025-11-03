#ifndef RVV_1024_H
#define RVV_1024_H

#include <riscv_vector.h>
#include <stdint.h>

// Number of 64-bit elements in a 1024-bit vector
#define VLEN_1024_IN_64 16

// Function to load a 1024-bit vector (16 x 64-bit)
static inline void rvv_1024_load(int64_t* dest, const int64_t* src) {
    for (size_t i = 0; i < VLEN_1024_IN_64; ++i) {
        vint64m1_t v = vle64_v_i64m1(src + i, 1);
        vse64_v_i64m1(dest + i, v, 1);
    }
    asm volatile("fence");
}

// Function to store a 1024-bit vector (16 x 64-bit)
static inline void rvv_1024_store(int64_t* dest, const int64_t* src) {
    for (size_t i = 0; i < VLEN_1024_IN_64; ++i) {
        vint64m1_t v = vle64_v_i64m1(src + i, 1);
        vse64_v_i64m1(dest + i, v, 1);
    }
    asm volatile("fence");
}

// Function to add two 1024-bit vectors
static inline void rvv_1024_add(int64_t* dest, const int64_t* src1, const int64_t* src2) {
    for (size_t i = 0; i < VLEN_1024_IN_64; ++i) {
        vint64m1_t v1 = vle64_v_i64m1(src1 + i, 1);
        vint64m1_t v2 = vle64_v_i64m1(src2 + i, 1);
        vint64m1_t v_res = vadd_vv_i64m1(v1, v2);
        vse64_v_i64m1(dest + i, v_res, 1);
    }
}

// Function to multiply two 1024-bit vectors (element-wise)
static inline void rvv_1024_mul(int64_t* dest, const int64_t* src1, const int64_t* src2) {
    for (size_t i = 0; i < VLEN_1024_IN_64; ++i) {
        vint64m1_t v1 = vle64_v_i64m1(src1 + i, 1);
        vint64m1_t v2 = vle64_v_i64m1(src2 + i, 1);
        vint64m1_t v_res = vmul_vv_i64m1(v1, v2);
        vse64_v_i64m1(dest + i, v_res, 1);
    }
}

#endif // RVV_1024_H
