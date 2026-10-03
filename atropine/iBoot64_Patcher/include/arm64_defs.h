/*
 * Copyright 2026 ENI & LO. ARM64 Architecture Definitions for A9-A10X iBoot Patcher.
 * Ported from legacy ARM32 arm_defs.h.
 */

#ifndef ARM64_DEFS_H
#define ARM64_DEFS_H

#include <stdint.h>

#define ARM64_NOP                 0xD503201F

/* ARM64 Register Identifiers */
#define ARM64_REG_X0              0
#define ARM64_REG_XZR             31

/* ARM64 Instruction Bitfield Masks */
#define ARM64_MASK_BL             0xFC000000
#define ARM64_PATTERN_BL          0x94000000

#define ARM64_MASK_B              0xFC000000
#define ARM64_PATTERN_B           0x14000000

#define ARM64_MASK_LDR_LIT        0xFF000000
#define ARM64_PATTERN_LDR_LIT     0x58000000

/* Structure representing an ARM64 Branch with Link (BL) instruction */
struct arm64_BL {
    uint32_t imm26 : 26;
    uint32_t op    : 6;
} __attribute__((packed));

/* Structure representing an ARM64 Unconditional Branch (B) instruction */
struct arm64_B {
    uint32_t imm26 : 26;
    uint32_t op    : 6;
} __attribute__((packed));

/* Structure representing an ARM64 Load Register (Literal) instruction */
struct arm64_LDR_lit {
    uint32_t rt    : 5;
    uint32_t imm19 : 19;
    uint32_t opc   : 2;
    uint32_t op    : 6;
} __attribute__((packed));

/* Structure representing an ARM64 ADRP instruction */
struct arm64_ADRP {
    uint32_t rd    : 5;
    uint32_t immlo : 2;
    uint32_t op    : 1;
    uint32_t immhi : 19;
    uint32_t base_op : 5;
} __attribute__((packed));

#endif /* ARM64_DEFS_H */
