#ifndef GEMMINI_ROCC_H
#define GEMMINI_ROCC_H

#include <stdint.h>

// Correct RoCC instruction encoding using the .insn directive for CUSTOM_0 opcode.
// funct: 7-bit function code for the co-processor
// rs1, rs2: source registers
// rd: destination register (x0 if not used)
#define ROCC_INSTRUCTION(funct, rd, rs1, rs2) \
    asm volatile( \
        ".insn r CUSTOM_0, 0, " #funct ", " #rd ", %0, %1" \
        : : "r"(rs1), "r"(rs2) : "memory" \
    )

// Gemmini Function Codes (funct) - based on public Gemmini documentation
#define GEMMINI_FUNC_CONFIG_LOAD  0
#define GEMMINI_FUNC_CONFIG_STORE 1
#define GEMMINI_FUNC_CONFIG_EX    2
#define GEMMINI_FUNC_LOAD         3
#define GEMMINI_FUNC_STORE        4
#define GEMMINI_FUNC_COMPUTE      5
#define GEMMINI_FUNC_PRELOAD      6

// Inline functions for core Gemmini operations
static inline void gemmini_write_tile(uint64_t src_addr, uint64_t dest_scratchpad_addr) {
    ROCC_INSTRUCTION(GEMMINI_FUNC_LOAD, x0, src_addr, dest_scratchpad_addr);
    asm volatile("fence");
}

static inline void gemmini_read_tile(uint64_t src_scratchpad_addr, uint64_t dest_addr) {
    ROCC_INSTRUCTION(GEMMINI_FUNC_STORE, x0, src_scratchpad_addr, dest_addr);
    asm volatile("fence");
}

static inline void gemmini_execute_gemm(uint64_t A_addr, uint64_t B_addr, uint64_t C_addr) {
    ROCC_INSTRUCTION(GEMMINI_FUNC_COMPUTE, x0, A_addr, B_addr);
    // In a real scenario, C_addr would be configured separately or passed here.
    asm volatile("fence");
}

#endif // GEMMINI_ROCC_H
