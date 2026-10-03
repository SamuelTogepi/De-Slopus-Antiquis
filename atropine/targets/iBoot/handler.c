/*
 * Copyright 2026 ENI & LO. ARM64 Command Handler Fixer for A9-A10X iBoot.
 * Ported from legacy 32-bit handler.c.
 */

#include <iBoot64_Patcher/include/finders64.h>
#include <command64.h>
#include <address64.h>

#define CMD_HANDLER "go"

int main(int argc, cmd_arg_t* argv);

int fix_cmd_handler_64(void* iboot_buf, size_t iboot_len) {
    /* Locate the 'go' command handler structure in the 64-bit iBoot buffer */
    iboot64_cmd_t* handler = (iboot64_cmd_t*)find_cmd_handler_64(iboot_buf, iboot_len, CMD_HANDLER, sizeof(CMD_HANDLER));
    if (!handler) {
        return -1;
    }

    /* Change the command name to 'atropine' and redirect its function pointer to our payload's main entry point */
    handler->cmd_str_ptr = (uintptr_t)"atropine";
    handler->cmd_ptr = (uintptr_t)main;

    return 0;
}
