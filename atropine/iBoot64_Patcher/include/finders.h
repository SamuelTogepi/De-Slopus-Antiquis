/*
 * Copyright 2026 ENI & LO. ARM64 Finders Header for A9-A10X iBoot Patcher.
 * Ported from legacy ARM32 finders.h.
 */

#ifndef FINDERS64_H
#define FINDERS64_H

#include <stddef.h>
#include <stdint.h>
#include <iBoot64_Patcher/include/iBoot64Patcher.h>

void* find_cmd_handler_64(void* iboot_buf, size_t iboot_len, const char* cmd_name, size_t cmd_len);
void* find_jumpto_64(struct iboot_img* iboot_in);
void* find_bl_verify_shsh_64(struct iboot_img* iboot_in);
void* find_bl_verify_shsh_a9_a10(struct iboot_img* iboot_in);
void* find_bl_verify_shsh_generic_64(struct iboot_img* iboot_in);
void* find_bl_verify_shsh_insn_64(struct iboot_img* iboot_in, void* pc);
void* find_verify_shsh_top_64(void* ptr);

/* Additional helper declarations for ARM64 pattern scanning */
void* arm64_find_jumpto_pattern(void* buf, size_t len);
bool arm64_validate_jumpto_context(void* candidate);
void* find_next_ARM64_literal_reference(void* buf, size_t len, const char* str, size_t str_len);
void* find_next_bl_insn_to_64(struct iboot_img* iboot_in, uint32_t addr);

#endif /* FINDERS64_H */
