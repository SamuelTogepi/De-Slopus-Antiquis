/*
 * Copyright 2026 ENI & LO. Main Header for ARM64 iBoot Patcher (A9-A10X).
 * Ported from legacy ARM32 iBoot32Patcher.h.
 */

#ifndef IBOOT64PATCHER_H
#define IBOOT64PATCHER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define bswap32 __builtin_bswap32
#define bswap16 __builtin_bswap16
#define bswap64 __builtin_bswap64

#define GET_IBOOT_FILE_OFFSET_64(iboot_in, x) ((uintptr_t)(x) - (uintptr_t)(iboot_in)->buf)
#define GET_IBOOT_ADDR_64(iboot_in, x) (((uintptr_t)(x) - (uintptr_t)(iboot_in)->buf) + get_iboot_base_address_64((iboot_in)->buf))

#define IMAGE4_MAGIC 'Img4'
#define IBOOT_VERS_STR_OFFSET_64 0x400  // Adjusted for 64-bit Mach-O headers in A9-A10X iBoot
#define TRAMPOLINE_OFFSET_64     0x400  // Free space slot for ARM64 trampoline injection
#define JUMPTO_ADDR_OFFSET_64    0x18   // Offset inside trampoline payload for branch target patching

struct iboot_img {
    void* buf;
    size_t len;
    uint32_t VERS;
} __attribute__((packed));

struct iboot64_cmd_t {
    uintptr_t cmd_str_ptr;
    uintptr_t cmd_ptr;
    uintptr_t cmd_desc_str_ptr;
} __attribute__((packed));

struct iboot_interval {
    int low, high, os;
} __attribute__((packed));

static const struct iboot_interval iboot_intervals[] = {
    {3403, 4000, 10}, // iOS 10 - A9/A10X
    {4400, 5000, 11}, // iOS 11 - A9/A10X
    {5400, 6000, 12}, // iOS 12 - A9/A10X
    {6500, 7200, 13}, // iOS 13 - A9/A10X
    {7500, 8500, 14}, // iOS 14 - A9/A10X
};

uint32_t get_iboot_base_address_64(void* iboot_buf);
int patch_iboot_64(void* buf, size_t len);

#endif /* IBOOT64PATCHER_H */
