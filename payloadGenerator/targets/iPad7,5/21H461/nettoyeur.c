/*
 * Copyright 2026 ENI & LO. Nettoyeur De Rebus Antiquis Patcher for iPad7,5 (A10).
 * Ported from legacy 32-bit Nettoyeur.c.
 */

#include <stdint.h>
#include <target64.h>

void _start(void) {
    uintptr_t base_address = IBOOT_BASE_ADDR_64;

    /* 
     * ARM64 Nettoyeur memory patch list for iPad7,5 iBoot runtime environment.
     * Offsets and patch values correspond to the 64-bit binary diff map 
     * generated via the Atropine shell 'diff' command.
     */
    
    *(uint32_t*)(base_address + 0x0004333C) = 0x9FF3727E; 
    *(uint32_t*)(base_address + 0x00043340) = 0x80000000; 
    *(uint32_t*)(base_address + 0x00046198) = 0x0E000000; 
    *(uint32_t*)(base_address + 0x000461A0) = 0x0E000001; 
    *(uint32_t*)(base_address + 0x00046320) = 0xFFFFFFFF; 
    *(uint32_t*)(base_address + 0x0004633C) = 0x00000000; 
    *(uint32_t*)(base_address + 0x00046340) = 0xFFFFFFFF; 
    *(uint32_t*)(base_address + 0x00046344) = 0x00000000; 
    *(uint32_t*)(base_address + 0x0004634C) = 0x00000000; 
    *(uint32_t*)(base_address + 0x00046350) = 0x00000001; 
    *(uint32_t*)(base_address + 0x00046354) = (uint32_t)(base_address + 0x46354); 
    *(uint32_t*)(base_address + 0x00046358) = (uint32_t)(base_address + 0x46354); 
    *(uint32_t*)(base_address + 0x0004638C) = (uint32_t)(base_address + 0x4638C); 
    *(uint32_t*)(base_address + 0x00046390) = (uint32_t)(base_address + 0x4638C); 
    *(uint32_t*)(base_address + 0x00046394) = (uint32_t)(base_address + 0x46394); 
    *(uint32_t*)(base_address + 0x00046398) = (uint32_t)(base_address + 0x46394); 
    *(uint32_t*)(base_address + 0x00046470) = 0xFFFFFFFF; 
    *(uint32_t*)(base_address + 0x00046474) = 0xFFFFFFFF; 
    *(uint32_t*)(base_address + 0x00046478) = 0x00000000; 
    *(uint32_t*)(base_address + 0x00046490) = (uint32_t)(base_address + 0x464B0); 
    *(uint32_t*)(base_address + 0x000464A0) = (uint32_t)(base_address + 0x464A0); 
    *(uint32_t*)(base_address + 0x000464A4) = (uint32_t)(base_address + 0x464A0); 
    *(uint32_t*)(base_address + 0x000464A8) = (uint32_t)(base_address + 0x464A8); 
    *(uint32_t*)(base_address + 0x000464AC) = (uint32_t)(base_address + 0x464A8); 
    *(uint32_t*)(base_address + 0x000464C4) = 0x00000002; 

    /* Zero out designated configuration blocks */
    for (uintptr_t i = 0x000464CC; i <= 0x00046620; i += 4) {
        *(uint32_t*)(base_address + i) = 0x00000000;
    }

    *(uint32_t*)(base_address + 0x00046650) = 0xFFFFFFFF; 
    *(uint32_t*)(base_address + 0x00046D20) = 0x00004000; 
    *(uint32_t*)(base_address + 0x00046D24) = 0x00000000; 
    *(uint32_t*)(base_address + 0x00046D28) = 0x01000000; 
}
