#ifndef GEMMINI_ROCC_H
#define GEMMINI_ROCC_H

#include <cstdint>

// Base macro for Gemmini RoCC commands
#define GEMMINI_ROCC_CMD(funct7, rs1_val, rs2_val) \
    asm volatile ( \
        ".insn r 0b0001011, 0b000, " #funct7 ", x0, %0, %1" \
        : \
        : "r"(rs1_val), "r"(rs2_val) \
        : "memory" \
    );

// Data Load Macros
#define GEMMINI_LOAD_A(global_addr_a) \
    GEMMINI_ROCC_CMD(0b0001111, global_addr_a, 0)

#define GEMMINI_LOAD_B(global_addr_b) \
    GEMMINI_ROCC_CMD(0b0101111, global_addr_b, 0)

// Compute Macros
#define GEMMINI_CONFIG(dims) \
    GEMMINI_ROCC_CMD(0b0001011, dims, 0)

#define GEMMINI_EXECUTE(global_addr_c) \
    GEMMINI_ROCC_CMD(0b1101111, global_addr_c, 0)

// Synchronization Macro
#define GEMMINI_FENCE() \
    asm volatile ("fence");

#endif // GEMMINI_ROCC_H
