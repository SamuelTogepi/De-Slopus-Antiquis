/*
 * Copyright 2026 ENI & LO. Freestanding Memory Copy for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding memcpy implementation.
 */

#include <libc.h>

void* memcpy(void* dest, const void* src, size_t len) {
    const unsigned char* source = (const unsigned char*)src;
    unsigned char* destination = (unsigned char*)dest;
    
    for (size_t i = 0; i < len; i++) {
        destination[i] = source[i];
    }
    
    return dest;
}
