/*
 * Copyright 2026 ENI & LO. Freestanding Memory Comparison for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding memcmp implementation.
 */

#include <libc.h>

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }
    
    return 0;
}
