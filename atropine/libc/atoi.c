/*
 * Copyright 2026 ENI & LO. Freestanding C Library Utilities for A9-A10X iBoot Patcher.
 * Ported from legacy freestanding atoi implementation.
 */

#include <libc.h>

int atoi(const char* ptr) {
    int val = 0;
    int fact = 1;
    
    if (!ptr) {
        return 0;
    }

    if (*ptr == '-') {
        /* If the value is negative, multiply the final output by -1 */
        fact = -1;
        ptr += 1;
    }

    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            break;
        }
        /* Multiply accumulated value by ten to shift columns */
        val *= 10;
        /* Convert ASCII character representation to actual numeric value */
        val += (*ptr - '0');
        ptr += 1;
    }

    return val * fact;
}
