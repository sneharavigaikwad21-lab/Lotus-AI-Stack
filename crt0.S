#
# PROJECT AGNI (V2.2) - C-RUNTIME STARTUP (crt0.S)
# Forged by: Integration Architect (The Foundry)
# Per "Perplexity-Validator" report (Minor Issue #2)
#

.section .init, "ax"  /* Place this in the .init section */
.global _start        /* Export the _start symbol */

_start:
    # 1. Initialize Stack Pointer (sp)
    # This is the "Golden" fix. It loads the "_stack_start"
    # address (from the linker script) into the sp register.
    .option push
    .option norelax
    la sp, _stack_start
    .option pop

    # 2. Zero the BSS section
    # (Uses _bss_start and _bss_end from linker script)
    la a0, _bss_start
    la a1, _bss_end
    j .L_bss_loop_check
.L_bss_loop:
    sd zero, 0(a0)  /* Store 8 bytes (doubleword) */
    addi a0, a0, 8
.L_bss_loop_check:
    blt a0, a1, .L_bss_loop

    # 3. Call C++ Global Constructors (Validator Fix)
    call __libc_init_array

    # 4. Call main
    call main

    # 5. If main returns, loop forever (safety)
.L_infinite_loop:
    j .L_infinite_loop
