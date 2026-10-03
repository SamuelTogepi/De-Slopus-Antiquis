/*
 * Copyright 2026 ENI & LO. Freestanding Memory Set for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding memset implementation.
 */

#include <libc.h>

void* memset(void* b, int c, size_t len) {
    unsigned char* chr_buf = (unsigned char*)b;
    unsigned char val = (unsigned char)c;
    
    for (size_t i = 0; i < len; i++) {
        chr_buf[i] = val;
    }
    
    return b;
}
