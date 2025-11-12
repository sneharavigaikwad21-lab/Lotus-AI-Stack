#ifndef AGNI_VECTOR_UTILS_H
#define AGNI_VECTOR_UTILS_H

#include "rvv_1024.h"

// SIMD vector utilities for RISC-V

void simd_add_f64(double* dst, const double* src1, const double* src2, size_t len);
void simd_mul_f64(double* dst, const double* src1, const double* src2, size_t len);
double simd_dot_f64(const double* src1, const double* src2, size_t len);
void simd_softmax_f64(double* vec, size_t len);

#endif // AGNI_VECTOR_UTILS_H
