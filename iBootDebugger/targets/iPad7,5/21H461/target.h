/*
 * Copyright 2026 ENI & LO. ARM64 Target Configuration Header for iPad7,5 (6th gen iPad, A10, Wi-Fi).
 * Ported from legacy 32-bit iPad2,1 target.h.
 */

#ifndef TARGET64_H
#define TARGET64_H

#include <stdint.h>

#define READP_BP_OFF_64          0x00001000  // Dynamic offset placeholder for iPad7,5 iBoot
#define READP_SAFE_REG_64        19          // X19

#define MEMALIGN_START_BP_OFF_64 0x00001200
#define MEMALIGN_START_SAFE_REG_64 20        // X20

#define MEMALIGN_LOOP_BP_OFF_64  0x00001400
#define MEMALIGN_LOOP_SAFE_REG_64 21       // X21

#define MEMALIGN_END_BP_OFF_64   0x00001600
#define MEMALIGN_END_SAFE_REG_64  22        // X22

/* Don't define any of this for the iBoot payload generator. */
#ifndef DEBUG_IBOOT_BUILDER_64

#define IBOOT_BASE_ADDR_64       0x180000000ULL /* Standard A10 iBoot load base map */

/* A Stack Pointer threshold near the bin list, used to reduce noise from memalign breakpoints. */
#define SP_NEAR_BIN_LIST_64      0x1800FF000ULL

/* Registers for common 64-bit handlers */
#define READP_SP_REG_64          "x19"
#define STACK_DUMP_SP_REG_64     "x20"

typedef int (*printf_t)(const char* fmt, ...);

/* Dynamic or symbol-resolved printf pointer for iPad7,5 iBoot runtime */
extern const printf_t printf;

__attribute__((always_inline)) static inline void fix_printf_64(void) {
    /* ARM64 pointer/GOT relocation adjustments for printf format strings inside iPad7,5 iBoot */
    // Patches applied dynamically via finder/relocation engine
}

__attribute__((always_inline)) static inline void memalign_start_handler_64(void) {
    register uint64_t reg_x0 __asm("x0");
    uint64_t x0 = reg_x0;
    register uint64_t reg_sp __asm("x20"); /* Contains SP */
    uint64_t sp = reg_sp;
    register uint64_t reg_x8 __asm("x8"); /* Heap bin table reference */
    uint64_t x8 = reg_x8;
    
    uint64_t sb = 0x20 - x0;
    uint64_t inferred = x8 + (sb << 3); /* 64-bit pointer scaling */
    uint64_t x4 = inferred + 0x30;     /* Next Bin structure offset */

    if (sp > SP_NEAR_BIN_LIST_64) {
        return;
    }

    // printf("_memalign (iPad7,5): sp=0x%016llx, x8=0x%016llx, sb=0x%016llx, x4=0x%016llx\n", sp, x8, sb, x4);
}

__attribute__((always_inline)) static inline void memalign_loop_handler_64(void) {
    register uint64_t reg_sp __asm("x21");
    uint64_t sp = reg_sp;
    register uint64_t reg_x0 __asm("x0"); /* Current Chunk */
    uint64_t x0 = reg_x0;
    register uint64_t reg_x1 __asm("x1"); /* Required Chunk Size */
    uint64_t x1 = reg_x1;

    if (sp > SP_NEAR_BIN_LIST_64) {
        return;
    }

    // printf("_memalign loop (iPad7,5): sp=0x%016llx, x0=0x%016llx, x1=0x%016llx\n", sp, x0, x1);
}

__attribute__((always_inline)) static inline void memalign_end_handler_64(void) {
    register uint64_t reg_sp __asm("x22");
    uint64_t sp = reg_sp;
    register uint64_t reg_x8 __asm("x8");
    uint64_t x8 = reg_x8;

    if (sp > SP_NEAR_BIN_LIST_64) {
        return;
    }

    // printf("_memalign end (iPad7,5): sp=0x%016llx, x8=0x%016llx\n", sp, x8);
}

#endif /* DEBUG_IBOOT_BUILDER_64 */

#endif /* TARGET64_H */
