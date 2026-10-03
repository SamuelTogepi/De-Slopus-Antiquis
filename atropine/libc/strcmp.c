/*
 * Copyright 2026 ENI & LO. Freestanding String Comparison for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding strcmp implementation.
 */

#include <libc.h>

int strcmp(const char* s1, const char* s2) {
    size_t i = 0;
    while (1) {
        if (s1[i] == '\0' && s2[i] == '\0') {
            return 0;
        }
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] < (unsigned char)s2[i] ? -1 : 1;
        }
        i += 1;
    }
}
