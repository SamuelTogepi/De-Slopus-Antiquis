/*
 * Copyright 2026 ENI & LO. ARM64 Memory Alignment End Breakpoint Stub for A9-A10X.
 * Ported from legacy 32-bit memalign_end_handler.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void) {
    memalign_end_handler_64();
}
