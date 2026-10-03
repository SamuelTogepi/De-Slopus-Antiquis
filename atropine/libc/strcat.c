/*
 * Copyright 2026 ENI & LO. Freestanding Serial Output Hook for A9-A10X iBoot Patcher.
 */

#include <libc.h>

putchar_t _putchar = NULL;

void set_putchar(putchar_t new_putchar) {
    _putchar = new_putchar;
}
