#ifndef NOC_HAL_H
#define NOC_HAL_H

#include <cstdint>

// Golden Knowledge (TRM v2.0 Sec 5.0)
// The hardware descriptor for a Network-on-Chip (NoC) copy command.
struct NocCopyDescriptor {
    void* source_addr;
    void* dest_addr;
    int size_bytes;
};

// Golden Knowledge (TRM v2.0 Sec 6.0)
// Memory-mapped registers for the NoC interface.
#define NOC_SEND_CMD  ((volatile uint64_t*)0x40000000)
#define NOC_STATUS    ((volatile uint64_t*)0x40000008)
#define NOC_IRQ_ENABLE ((volatile uint64_t*)0x4000000C)

// Implements a simple, blocking data copy using the NoC hardware.
// This function is intended for use by compiler-generated code (like TVM)
// that requires a simple, synchronous data transfer API.
inline void noc_copy_blocking(void* dest, void* src, int size_bytes) {
    // 1. Create a descriptor for the copy operation on the stack.
    static NocCopyDescriptor descriptor;
    descriptor.source_addr = src;
    descriptor.dest_addr = dest;
    descriptor.size_bytes = size_bytes;

    // 2. Issue the command by writing the address of the descriptor
    //    to the NoC's command register.
    *NOC_SEND_CMD = (uint64_t)&descriptor;

    // 3. Poll the status register until the hardware reports completion.
    //    (Read 0x1 = BUSY, 0x0 = DONE). This makes the call "blocking".
    while (*NOC_STATUS != 0) {
        // Busy-wait
    }
}

#endif // NOC_HAL_H
