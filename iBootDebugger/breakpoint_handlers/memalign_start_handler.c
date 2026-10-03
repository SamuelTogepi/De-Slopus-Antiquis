/*
 * Copyright 2026 ENI & LO. ARM64 Memory Alignment Start Breakpoint Stub for A9-A10X.
 * Ported from legacy 32-bit memalign_start_handler.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void) {
    memalign_start_handler_64();
}
