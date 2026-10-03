/*
 * Copyright 2026 ENI & LO. ARM64 Command Table Iterator for A9-A10X iBoot.
 * Ported from legacy 32-bit command routing engine.
 */

#include <command64.h>
#include <address64.h>
#include <libc/libc.h>

int iterate_commands_64(int argc, cmd_arg_t* argv) {
    /* In ARM64 iBoot binaries, command structures contain 64-bit aligned pointers 
     * for handler functions and string references. */
    cmd_handler_t* cmd = (cmd_handler_t*)&commands_64;

    /* Ensure the command search stays within the bounds of the active command table section */
    while (cmd != (cmd_handler_t*)&ecommands_64) {
        
        if (cmd->name && argv && argv[0].string) {
            /* Check if the current table entry matches the requested command string */
            if (!strcmp(cmd->name, argv[0].string)) {
                return cmd->func(argc - 1, &argv[1]);
            }
        }

        /* Pointer arithmetic automatically scales by sizeof(cmd_handler_t) in 64-bit memory */
        cmd += 1;
    }

    printf("Command not found, try help for a list of possible commands\n");
    return -1;
}
