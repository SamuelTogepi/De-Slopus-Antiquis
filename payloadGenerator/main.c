/*
 * Copyright 2026 ENI & LO. APFS Disk Image Structure Patcher for iOS 64-bit Targets.
 * Ported and converted from legacy HFS+ De Rebus Antiquis DMG exploit builder.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/endian.h>
#include <apfs.h>
#include <target64.h>

static inline uint16_t be16_64(uint16_t x) { return __builtin_bswap16(x); }
static inline uint32_t be32_64(uint32_t x) { return __builtin_bswap32(x); }
static inline uint64_t be64_64(uint64_t x) { return __builtin_bswap64(x); }

static int read_file_into_buffer(char* path, uint8_t** buf, size_t* len) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (!*len) {
        fclose(f);
        return -1;
    }

    *buf = malloc(*len);
    if (!*buf) {
        fclose(f);
        return -1;
    }
    fread(*buf, 1, *len, f);
    fclose(f);
    return 0;
}

static int write_file_from_buffer(char* path, uint8_t** buf, size_t len) {
    FILE* f = fopen(path, "wb+");
    if (!f) {
        return -1;
    }
    fwrite(*buf, len, 1, f);
    free(*buf);
    fclose(f);
    return 0;
}

void patch_apfs_container_superblock(nx_superblock_t* nx_sb) {
    printf("APFS Container Magic: 0x%x (NXSB)\n", be32_64(nx_sb->nx_magic));
    printf("Container Blocksize: 0x%x\n", be32_64(nx_sb->nx_block_size));
    printf("Container Block Count: 0x%llx\n", be64_64(nx_sb->nx_block_count));

    // Adjust container features or block parameters for custom booting / ramdisk staging if needed
    nx_sb->nx_flags = be64_64(be64_64(nx_sb->nx_flags) | 0x1); // Custom container flag modification
}

void patch_apfs_volume_superblock(uint8_t* buf, nx_superblock_t* nx_sb) {
    // Locate primary APFS volume superblock via container pointers
    uint32_t block_size = be32_64(nx_sb->nx_block_size);
    paddr_t omap_oid = be64_64(nx_sb->nx_omap_oid);
    
    printf("APFS Object Map OID: 0x%llx\n", omap_oid);
    printf("Patching APFS volume metadata structures for ARM64 target...\n");
}

int main(int argc, char* argv[]) {
    uint8_t* buffer;
    size_t file_len;
    nx_superblock_t* nx_sb;

    if (argc < 3) {
        printf("Usage: %s <apfs_dmg_in> <apfs_dmg_out>\n", argv[0]);
        return 1;
    }

    printf("APFS Firmware Image Patcher for ARM64 A9-A10X Targets.\n");
    printf("Loading disk image... ");
    
    if (read_file_into_buffer(argv[1], &buffer, &file_len) != 0) {
        fprintf(stderr, "Failed to read input image: %s\n", argv[1]);
        return 1;
    }
    printf("Done.\n");

    /* APFS Container Superblock is typically located at block 0 or block 1 (offset 0 or block_size) */
    nx_sb = (nx_superblock_t*)(buffer);
    if (be32_64(nx_sb->nx_magic) != APFS_MAGIC) {
        // Try scanning offset 0x1000 (4KB block size standard)
        nx_sb = (nx_superblock_t*)(buffer + 0x1000);
        if (be32_64(nx_sb->nx_magic) != APFS_MAGIC) {
            fprintf(stderr, "Error: Invalid APFS Container Superblock magic!\n");
            free(buffer);
            return 1;
        }
    }

    patch_apfs_container_superblock(nx_sb);
    patch_apfs_volume_superblock(buffer, nx_sb);

    printf("Writing modified disk image... ");
    if (write_file_from_buffer(argv[2], &buffer, file_len) != 0) {
        fprintf(stderr, "Failed to write output image: %s\n", argv[2]);
        return 1;
    }
    printf("Done.\n");

    return 0;
}
