#include "vector_utils.h"
#include "common.h"
#include <math.h>

void simd_add_f64(double* dst, const double* src1, const double* src2, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dst[i] = src1[i] + src2[i];
    }
}

void simd_mul_f64(double* dst, const double* src1, const double* src2, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dst[i] = src1[i] * src2[i];
    }
}

double simd_dot_f64(const double* src1, const double* src2, size_t len) {
    double result = 0.0;
    for (size_t i = 0; i < len; i++) {
        result += src1[i] * src2[i];
    }
    return result;
}

void simd_softmax_f64(double* vec, size_t len) {
    // Find max for numerical stability
    double max_val = vec[0];
    for (size_t i = 1; i < len; i++) {
        if (vec[i] > max_val) max_val = vec[i];
    }

    // Compute exp and sum
    double sum = 0.0;
    for (size_t i = 0; i < len; i++) {
        vec[i] = exp(vec[i] - max_val);
        sum += vec[i];
    }

    // Normalize
    for (size_t i = 0; i < len; i++) {
        vec[i] /= sum;
    }
}
