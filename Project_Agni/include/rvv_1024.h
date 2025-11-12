#ifndef RVV_1024_H
#define RVV_1024_H

#include <riscv_vector.h>
#include <stdint.h>
#include <cstddef>

// RISC-V Vector (RVV) 1.0 utilities for 1024-bit vectors
// Optimized for AGNI chip

typedef struct {
    double* data;
    size_t length;
} Vec1024_f64;

// Vector creation and destruction
Vec1024_f64* vec1024_create(size_t length);
void vec1024_free(Vec1024_f64* vec);

// Vector operations
void vec1024_add(Vec1024_f64* dest, const Vec1024_f64* src1, const Vec1024_f64* src2);
void vec1024_mul(Vec1024_f64* dest, const Vec1024_f64* src1, const Vec1024_f64* src2);
void vec1024_dot(double* result, const Vec1024_f64* src1, const Vec1024_f64* src2);
void vec1024_softmax(Vec1024_f64* vec);

// Matrix operations
void vec1024_matmul(double* C, const double* A, const double* B,
                    size_t M, size_t N, size_t K);

#endif // RVV_1024_H
