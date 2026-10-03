/*
 * Copyright 2026 ENI & LO. ARM64 Memory Alignment Loop Breakpoint Stub for A9-A10X.
 * Ported from legacy 32-bit memalign_loop_handler.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void) {
    memalign_loop_handler_64();
}
