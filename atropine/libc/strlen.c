/*
 * Copyright 2026 ENI & LO. Freestanding String Length for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding strlen implementation.
 */

#include <libc.h>

size_t strlen(const char* s) {
    if (!s) {
        return 0;
    }
    const char* p = s;
    while (*p != '\0') {
        p++;
    }
    return (size_t)(p - s);
}
