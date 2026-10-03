/*
 * Copyright 2026 ENI & LO. ARM64 Instruction Macro Header for A9-A10X Patcher.
 * Ported from legacy 32-bit asm.h.
 */

#ifndef ASM64_H
#define ASM64_H

#include <stdint.h>
#include <sys/endian.h>

static inline uint16_t be16_64(uint16_t x) { return __builtin_bswap16(x); }
static inline uint32_t be32_64(uint32_t x) { return __builtin_bswap32(x); }
static inline uint64_t be64_64(uint64_t x) { return __builtin_bswap64(x); }

/* ARM64 Register Aliases (X0 - X30, SP, XZR) */
#define X0  (0)
#define X1  (1)
#define X2  (2)
#define X3  (3)
#define X4  (4)
#define X5  (5)
#define X6  (6)
#define X7  (7)
#define X8  (8)
#define X9  (9)
#define X10 (10)
#define X11 (11)
#define X12 (12)
#define X13 (13)
#define X14 (14)
#define X15 (15)
#define X16 (16)
#define X17 (17)
#define X18 (18)
#define X19 (19)
#define X20 (20)
#define X21 (21)
#define X22 (22)
#define X23 (23)
#define X24 (24)
#define X25 (25)
#define X26 (26)
#define X27 (27)
#define X28 (28)
#define X29 (29) // Frame Pointer (FP)
#define X30 (30) // Link Register (LR)
#define SP  (31) // Stack Pointer / Zero Register context

#define ARM64_NOP              (uint32_t)0xD503201F
#define ARM64_RET              (uint32_t)0xD65F03C0

/* Helper to write 32-bit ARM64 instructions into binary buffer */
static inline void write_insn_32_64(uint8_t* buf, size_t* index, uint32_t insn) {
    *(uint32_t*)&buf[*index] = insn;
    *index += sizeof(uint32_t);
}

#define WRITE_INSN_64(buf, i, insn) write_insn_32_64(buf, &i, (uint32_t)(insn))

/* Calculate ARM64 Branch with Link (BL) instruction encoding for given position and target */
static inline uint32_t make_bl_64(int64_t pos, int64_t tgt) {
    int64_t delta = tgt - pos;
    // ARM64 BL immediate is a 26-bit signed word offset relative to PC
    int32_t imm26 = (int32_t)(delta >> 2);
    
    // BL opcode is 0x94000000 with 26-bit offset field
    return (uint32_t)(0x94000000 | (imm26 & 0x03FFFFFF));
}

#endif /* ASM64_H */
