/*
 * Copyright 2026 ENI & LO. ARM64 Generic Breakpoint Handler for A9-A10X.
 * Ported from legacy 32-bit generic_handler.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void) {
    register uint64_t reg_x0 __asm("x0");
    uint64_t x0 = reg_x0;
    register uint64_t reg_x1 __asm("x1");
    uint64_t x1 = reg_x1;
    register uint64_t reg_x2 __asm("x2");
    uint64_t x2 = reg_x2;
    register uint64_t reg_x3 __asm("x3");
    uint64_t x3 = reg_x3;
    register uint64_t reg_x4 __asm("x4");
    uint64_t x4 = reg_x4;
    register uint64_t reg_x5 __asm("x5");
    uint64_t x5 = reg_x5;
    register uint64_t reg_x6 __asm("x6");
    uint64_t x6 = reg_x6;
    register uint64_t reg_x7 __asm("x7");
    uint64_t x7 = reg_x7;
    register uint64_t reg_x8 __asm("x8");
    uint64_t x8 = reg_x8;
    register uint64_t reg_x9 __asm("x9");
    uint64_t x9 = reg_x9;
    register uint64_t reg_x10 __asm("x10");
    uint64_t x10 = reg_x10;
    register uint64_t reg_x11 __asm("x11");
    uint64_t x11 = reg_x11;
    register uint64_t reg_x12 __asm("x12");
    uint64_t x12 = reg_x12;
    register uint64_t reg_x13 __asm("x13");
    uint64_t x13 = reg_x13;
    register uint64_t reg_x14 __asm("x14");
    uint64_t x14 = reg_x14;
    register uint64_t reg_x15 __asm("x15");
    uint64_t x15 = reg_x15;
    register uint64_t reg_sp __asm("sp");
    uint64_t sp = reg_sp;
    register uint64_t reg_lr __asm("x30");
    uint64_t lr = reg_lr;

    printf("ARM64 Breakpoint hit! "
           "x0=0x%016llx, x1=0x%016llx, x2=0x%016llx, x3=0x%016llx, "
           "x4=0x%016llx, x5=0x%016llx, x6=0x%016llx, x7=0x%016llx, "
           "x8=0x%016llx, x9=0x%016llx, x10=0x%016llx, x11=0x%016llx, "
           "x12=0x%016llx, x13=0x%016llx, x14=0x%016llx, x15=0x%016llx, "
           "sp=0x%016llx, lr=0x%016llx\n",
           x0, x1, x2, x3, x4, x5, x6, x7,
           x8, x9, x10, x11, x12, x13, x14, x15,
           sp, lr);
}
