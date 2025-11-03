#ifndef RVV_1024_H
#define RVV_1024_H

// Define a 1024-bit vector type
typedef __attribute__((vector_size(128))) long v1024_t;

// Define some custom intrinsics for our 1024-bit vector type
#define rvv_1024_add(a, b) ((a) + (b))
#define rvv_1024_sub(a, b) ((a) - (b))
#define rvv_1024_mul(a, b) ((a) * (b))

// "Brute-Force Proof" intrinsics with memory fences
#define rvv_1024_load(ptr) ({ \
  v1024_t value; \
  asm volatile("vle1024.v %0, (%1); fence" : "=vr"(value) : "r"(ptr)); \
  value; \
})

#define rvv_1024_store(ptr, value) ({ \
  asm volatile("vse1024.v %0, (%1); fence" : : "vr"(value), "r"(ptr)); \
})

#endif // RVV_1024_H
