/*
 * PROJECT AGNI (V37 "ZERO-BUG" FIX) - GOLDEN KNOWLEDGE LINKER SCRIPT
 * Forged by: Integration Architect (The Foundry)
 * Target: CVA6S+ (rv6gc)
 * Status: V37 "HARD GO" - Corrects "Goliath Trap" (Bug #1: BSS/DATA in GDDR6)
 */

ENTRY(_start)

/* Define the complete v2.1 "Canonical Memory Map" */
MEMORY
{
    /* Per-Core Memory */
    CPU_RAM (rwx)     : ORIGIN = 0x00000000, LENGTH = 16M   /* .text, .data, .bss */
    WRENCH_L2 (rwx)   : ORIGIN = 0x20000000, LENGTH = 2M
    KEY_L2 (rwx)      : ORIGIN = 0x30000000, LENGTH = 2M
    NOC_REGS (rw)     : ORIGIN = 0x40000000, LENGTH = 64K

    /* Global Memory */
    GLOBAL_ACCEL (rw) : ORIGIN = 0x50000000, LENGTH = 256M
    GDDR6 (rwx)       : ORIGIN = 0x80000000, LENGTH = 16G   /* Models, heap, stack */
}

/* Define a 128K stack size for "Lotus OS" */
_STACK_SIZE = 128K;

SECTIONS
{
    /* --- V37 "GOLIATH TRAP" FIX (BUG #1) ---
     * .text, .data, and .bss are placed in CPU_RAM for 1-cycle
     * access. The GDDR6 path (100+ cycles) is ASSASSINATED.
     */

    /* SECTION 1: "Core Command" Code (in CPU_RAM) */
    .text :
    {
        KEEP(*(.init))  /* Startup code from crt0.S */
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > CPU_RAM

    /* C++ Runtime Support (in CPU_RAM) */
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > CPU_RAM

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > CPU_RAM

    /* SECTION 2: Initialized Data (in CPU_RAM) */
    .data :
    {
        . = ALIGN(8);
        *(.data)
        *(.data.*)
    } > CPU_RAM

    /* SECTION 3: Uninitialized Data (in CPU_RAM) */
    .bss :
    {
        . = ALIGN(8);
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
    } > CPU_RAM
    _bss_size = _bss_end - _bss_start;

    /* --- GLOBAL MEMORY SECTIONS --- */

    /* SECTION 4: Stitched IREE Model Binaries (in GDDR6) */
    .wrench_model_data : ALIGN(16)
    {
        *(.wrench_model_data)
    } > GDDR6

    .key_model_data : ALIGN(16)
    {
        *(.key_model_data)
    } > GDDR6

    /* SECTION 5: "Lotus OS" Heap (in GDDR6) */
    .heap (NOLOAD):
    {
        . = ALIGN(16);
        _heap_start = .;
        _heap_end = ORIGIN(GDDR6) + LENGTH(GDDR6) - _STACK_SIZE;
    } > GDDR6

    /* SECTION 6: "Lotus OS" Stack (in GDDR6) */
    .stack (NOLOAD):
    {
        . = ORIGIN(GDDR6) + LENGTH(GDDR6) - _STACK_SIZE;
        . = ALIGN(16);
        _stack_end = .;        /* Bottom of stack */
        . += _STACK_SIZE;      /* Reserve stack space */
        _stack_start = .;      /* Top of stack (SP init value) */
    } > GDDR6
}
