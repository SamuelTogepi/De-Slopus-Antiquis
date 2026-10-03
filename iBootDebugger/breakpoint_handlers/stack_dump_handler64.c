/*
 * Copyright 2026 ENI & LO. ARM64 Stack Dump Breakpoint Handler for A9-A10X.
 * Ported from legacy 32-bit stack_dump_handler.c.
 */

#include <stdint.h>
#include <stddef.h>
#include <target64.h>

void _start(void) {
    register uint64_t reg_sp __asm("sp");
    uint64_t sp = reg_sp;

    if (sp > SP_NEAR_BIN_LIST_64)
        return;

    fix_printf_64();
    printf("ARM64 STACK DUMP! sp=0x%016llx\n", sp);

    for (int64_t i = -0x20; i <= 0x20; i += 0x8) {
        uint64_t* off = (uint64_t*)(sp + i);
        printf("0x%016llx: 0x%016llx", (uint64_t)off, *off);
        
        uint64_t* ptr_val = (uint64_t*)*off;
        for (int j = 0; j < 5; j += 1) {
            // Check if pointer falls within the ARM64 iBoot text/data base range
            int is_addr = (((uintptr_t)ptr_val) & 0xFFFFFFFF00000000ULL) == (IBOOT_BASE_ADDR_64 & 0xFFFFFFFF00000000ULL);
            if (!is_addr)
                break;
            printf(" (0x%016llx)", (uint64_t)*ptr_val);
            ptr_val = (uint64_t*)*ptr_val;
        }
        printf("\n");
    }
}
