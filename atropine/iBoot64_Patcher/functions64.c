/*
 * Copyright 2026 ENI & LO. ARM64 Utility & Patcher Helpers for A9-A10X iBoot.
 * Ported from legacy ARM32 iBoot32Patcher helper codebase.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <include/functions64.h>
#include <include/iBoot64Patcher.h>

int64_t sign_extend_26_64(uint64_t x) {
    const int bits = 26;
    uint64_t m = 1ULL << (bits - 1);
    return (int64_t)((x ^ m) - m);
}

void build_bl_64(void* insn, void* target) {
    uintptr_t pc = (uintptr_t)insn;
    int64_t offset = (int64_t)((uintptr_t)target - pc);
    
    // ARM64 BL instruction encoding: op (6 bits) = 0b100101, imm26 (26 bits)
    uint32_t imm26 = ((uint32_t)(offset >> 2)) & 0x03FFFFFF;
    uint32_t* insn_ptr = (uint32_t*)insn;
    *insn_ptr = 0x94000000 | imm26;
}

void* bl_search_down_64(const void* start_addr, int len) {
    // Search for ARM64 BL instruction mask: op code 0x94000000 with mask 0xFC000000
    return pattern_search_64(start_addr, len, 0x94000000, 0xFC000000, 4);
}

void* b_search_down_64(const void* start_addr, int len) {
    // Search for ARM64 unconditional branch B instruction mask: op code 0x14000000 with mask 0xFC000000
    return pattern_search_64(start_addr, len, 0x14000000, 0xFC000000, 4);
}

void* find_next_bl_insn_to_64(struct iboot_img* iboot_in, uint32_t addr) {
    uint8_t* buf = (uint8_t*)iboot_in->buf;
    size_t len = iboot_in->len;

    for (size_t i = 0; i < len - sizeof(uint32_t); i += 4) {
        void* possible_bl = buf + i;
        uint32_t insn = *(uint32_t*)possible_bl;
        
        // Check if instruction is a BL (Branch with Link: top 6 bits = 37 / 0x25 -> 0x94000000 mask 0xFC000000)
        if ((insn & 0xFC000000) == 0x94000000) {
            void* bl_target = resolve_bl64(possible_bl);
            uint32_t resolved_offset = (uint32_t)((uintptr_t)GET_IBOOT_FILE_OFFSET(iboot_in, bl_target));
            if (resolved_offset == addr) {
                return possible_bl;
            }
        }
    }
    return NULL;
}

void* find_next_CMP_imm_insn_64(void* start, size_t len, uint8_t reg, uint16_t imm) {
    uint8_t* caddr = (uint8_t*)start;
    for (size_t i = 0; i < len; i += 4) {
        uint32_t insn = *(uint32_t*)(caddr + i);
        // ARM64 CMP (immediate) is alias for SUBS wzr/xzr, Rn, #imm (Op0/shift/imm12 encoding)
        // Checking base instruction patterns for immediate comparisons
        if ((insn & 0xFF800000) == 0xF1000000) { // SUBS immediate pattern for 64-bit
            uint32_t rn = insn & 0x1F;
            uint32_t imm12 = (insn >> 10) & 0xFFF;
            if (rn == reg && imm12 == imm) {
                return (void*)(caddr + i);
            }
        }
    }
    return NULL;
}

void* find_next_LDR_literal_insn_64(struct iboot_img* iboot_in, uint32_t value) {
    void* ldr_xref = (void*)memmem(iboot_in->buf, iboot_in->len, &value, sizeof(value));
    if (!ldr_xref) {
        return NULL;
    }
    return ldr_to_64(iboot_in, ldr_xref);
}

uint32_t get_iboot_base_address_64(void* iboot_buf) {
    // ARM64 iBoot header usually stores entry / base configuration parameters at fixed header offsets
    return *(uint32_t*)((uint8_t*)iboot_buf + 0x20) & ~0xFFFFF;
}

int get_os_version_64(struct iboot_img* iboot_in) {
    for (size_t i = 0; i < sizeof(iboot_intervals) / sizeof(struct iboot_interval); i++) {
        if (iboot_in->VERS >= iboot_intervals[i].low && iboot_in->VERS <= iboot_intervals[i].high) {
            return iboot_intervals[i].os;
        }
    }
    return 0;
}

void* iboot_memmem_64(struct iboot_img* iboot_in, void* pat) {
    uintptr_t new_pat = (uintptr_t)GET_IBOOT_ADDR(iboot_in, pat);
    return (void*)memmem(iboot_in->buf, iboot_in->len, &new_pat, sizeof(uintptr_t));
}

void* ldr_to_64(struct iboot_img* iboot_in, const void* loc) {
    uintptr_t target_addr = (uintptr_t)loc;
    uint8_t* buf = (uint8_t*)iboot_in->buf;
    size_t total_len = iboot_in->len;

    // In ARM64, LDR (literal) uses a PC-relative offset within +/- 1MB
    for (size_t i = 0; i < total_len; i += 4) {
        uint32_t insn = *(uint32_t*)(buf + i);
        // Check for ARM64 LDR literal instruction format: opc(2) 01 1f 0000 (mask 0xFF000000 -> 0x58000000 or 0x5c000000)
        if ((insn & 0x3F000000) == 0x18000000 || (insn & 0xFF000000) == 0x58000000) {
            int32_t imm19 = (int32_t)((insn >> 5) & 0x7FFFF);
            if (imm19 & 0x40000) imm19 |= ~0x7FFFF; // Sign extend 19-bit
            uintptr_t ldr_target = (uintptr_t)(buf + i) + (imm19 << 2);
            if (ldr_target == target_addr) {
                return (void*)(buf + i);
            }
        }
    }
    return NULL;
}

void* pattern_search_64(const void* addr, int len, int pattern, int mask, int step) {
    char* caddr = (char*)addr;
    if (len <= 0) return NULL;
    
    for (int i = 0; i < len; i += step) {
        uint32_t x = *(uint32_t*)(caddr + i);
        if ((x & mask) == (uint32_t)pattern) {
            return (void*)(caddr + i);
        }
    }
    return NULL;
}

void* arm64_frame_prologue_search_up(const void* start_addr, int len) {
    // Search upwards for ARM64 standard stack frame prologue: STP x29, x30, [sp, ...]
    char* caddr = (char*)start_addr;
    for (int i = 0; i < len; i += 4) {
        uint32_t insn = *(uint32_t*)(caddr - i);
        // STP pre-index or post-index pattern for x29, x30: 0xA9800000 mask 0xFFC00000 (approx)
        if ((insn & 0xFFC00000) == 0xA9800000 || (insn & 0xFFC00000) == 0xA9000000) {
            return (void*)(caddr - i);
        }
    }
    return NULL;
}

void* resolve_bl64(const void* bl) {
    uint32_t insn = *(uint32_t*)bl;
    // Extract 26-bit immediate offset from BL instruction
    int32_t imm26 = insn & 0x03FFFFFF;
    if (imm26 & 0x02000000) {
        imm26 |= ~0x03FFFFFF; // Sign extend 26-bit
    }
    int64_t offset = (int64_t)imm26 << 2;
    return (void*)((uintptr_t)bl + offset);
}
