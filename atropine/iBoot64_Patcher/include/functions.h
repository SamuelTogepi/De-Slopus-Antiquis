/*
 * Copyright 2026 ENI & LO. ARM64 Functions Header for A9-A10X iBoot Patcher.
 * Ported from legacy ARM32 functions.h.
 */

#ifndef FUNCTIONS64_H
#define FUNCTIONS64_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <iBoot64_Patcher/include/arm64_defs.h>
#include <iBoot64_Patcher/include/iBoot64Patcher.h>

#define MEMMEM_RELATIVE_64(iboot_in, bufstart, needle, needleLen) memmem(bufstart, iboot_in->len - ((char*)(bufstart) - (char*)iboot_in->buf), needle, needleLen)

int64_t sign_extend_26_64(uint64_t x);
void build_bl_64(void* insn, void* target);
void* bl_search_down_64(const void* start_addr, int len);
void* b_search_down_64(const void* start_addr, int len);
void* iboot_memmem_64(struct iboot_img* iboot_in, void* pat);
void* find_next_bl_insn_to_64(struct iboot_img* iboot_in, uint32_t addr);
void* find_next_CMP_imm_insn_64(void* start, size_t len, uint8_t reg, uint16_t imm);
void* find_next_LDR_literal_insn_64(struct iboot_img* iboot_in, uint32_t value);
uint32_t get_iboot_base_address_64(void* iboot_buf);
int get_os_version_64(struct iboot_img* iboot_in);
char* arm64_find_iboot_version_string(void* buf, size_t len);
void* ldr_to_64(struct iboot_img* iboot_in, const void* loc);
void* memstr(const void* mem, size_t size, const char* str);
void* pattern_search_64(const void* addr, int len, int pattern, int mask, int step);
void* arm64_frame_prologue_search_up(const void* start_addr, int len);
void* find_ticket_check_routine_64(struct iboot_img* iboot_in);
void* resolve_bl64(const void* bl);

#endif /* FUNCTIONS64_H */
