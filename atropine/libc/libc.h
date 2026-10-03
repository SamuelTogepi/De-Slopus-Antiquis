/*
 * Copyright 2026 ENI & LO. Freestanding C Library Header for A9-A10X iBoot Patcher.
 * Ported from legacy ARM32 libc.h.
 */

#ifndef LIBC64_H
#define LIBC64_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef void (*putchar_t)(char character);

void clear_icache_64(void);
void flush_cache_range_64(void* start_addr, size_t size);
int atoi(const char* ptr);
void* memcpy(void* dest, const void* src, size_t len);
void* memmem(const void* haystack, size_t hlen, const void* needle, size_t nlen);
int memcmp(const void* s1, const void* s2, size_t n);
void* memset(void* b, int c, size_t len);
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strcat(char* restrict s1, const char* restrict s2);
char* strncat(char* restrict s1, const char* restrict s2, size_t n);
void set_putchar(putchar_t new_putchar);
extern putchar_t _putchar;

#endif /* LIBC64_H */
