/*
 * Copyright 2026 ENI & LO. ARM64 Read Parameter Breakpoint Handler for A9-A10X.
 * Ported from legacy 32-bit readp_handler.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void *ih, void *buffer, long offset) {
    register uint64_t reg_sp __asm("sp");
    uint64_t sp = reg_sp;
    
    // In ARM64 calling conventions, extra arguments or stack frames are offset accordingly
    uint32_t length = *(uint32_t*)(sp + 0x20);

    printf("readp(%p, %p, 0x%lx, %u)\n", ih, buffer, offset, length);
}
