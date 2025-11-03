#ifndef GEMMINI_H
#define GEMMINI_H

#include <stdint.h>

#define ROCC_INSTRUCTION(opcode, rd, rs1, rs2, funct) \
    asm volatile(                                     \
        ".word ((" #funct " << 25) | (" #rs2 " << 20) | (" #rs1 " << 15) | (" #rd " << 7) | (" #opcode "));" \
        :                                             \
        :                                             \
        : "memory")

static inline void gemmini_mvin(uint64_t from, uint64_t to) {
    ROCC_INSTRUCTION(0, 0, from, to, 2);
    asm volatile("fence");
}

static inline void gemmini_mvout(uint64_t from, uint64_t to) {
    ROCC_INSTRUCTION(0, 0, from, to, 3);
    asm volatile("fence");
}

static inline void gemmini_compute_matmul(uint64_t A, uint64_t B, uint64_t C) {
    ROCC_INSTRUCTION(0, 0, A, B, 4);
    asm volatile("fence");
    ROCC_INSTRUCTION(0, 0, B, C, 5);
    asm volatile("fence");
}

#endif // GEMMINI_H
