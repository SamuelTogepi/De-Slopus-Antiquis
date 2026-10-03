/*
 * Copyright 2026 ENI & LO. ARM64 Patchers Header for A9-A10X iBoot Patcher.
 * Ported from legacy ARM32 patchers.h.
 */

#ifndef PATCHERS64_H
#define PATCHERS64_H

#include <iBoot64_Patcher/include/iBoot64Patcher.h>

#define DEFAULT_BOOTARGS_STR "rd=md0 nand-enable-reformat=1 -progress"
#define JUMPTO_ADDR_OFFSET_64 (trampoline_length_64 - (2 * sizeof(uint64_t)))

int patch_boot_args_64(struct iboot_img* iboot_in);
int patch_llb_load_64(struct iboot_img* iboot_in);
int patch_rsa_check_64(struct iboot_img* iboot_in);
int patch_command_handler_64(struct iboot_img* iboot_in);
int patch_ticket_check_64(struct iboot_img* iboot_in);

#endif /* PATCHERS64_H */
