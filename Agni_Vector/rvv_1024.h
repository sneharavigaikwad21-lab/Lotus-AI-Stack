#ifndef RVV_1024_H
#define RVV_1024_H

#include <riscv_vector.h>

// "Key" Engine VLEN (TRM v2.0, Sec 2.3)
#define AGNI_VLEN_BITS 1024

// "Key" Engine L2 SRAM (TRM v2.0, Sec 6.0)
#define AGNI_KEY_L2_BASE 0x30000000
#define AGNI_KEY_L2_SIZE 0x200000 // (2MB)

static inline void agni_memory_fence() {
  __asm__ volatile ("fence i\n fence rw, rw" ::: "memory");
}

// Sets VL for 64-bit elements, LMUL=8
static inline size_t agni_set_vl_e64m8(size_t avl) {
  return __riscv_vsetvl_e64m8(avl);
}

// Vector Load: 64-bit float, LMUL=8
static inline vfloat64m8_t agni_vload_e64m8(const double* base, size_t vl) {
  return __riscv_vle64_v_f64m8(base, vl);
}

// Vector Store: 64-bit float, LMUL=8
static inline void agni_vstore_e64m8(double* base, vfloat64m8_t value, size_t vl) {
  __riscv_vse64_v_f64m8(base, value, vl);
}

#endif // RVV_1024_H
