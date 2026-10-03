/*
 * Copyright 2026 ENI & LO. Apple File System (APFS) Structures Header for iOS Firmware Analysis.
 * Converted and redesigned from legacy hfs.h.
 */

#ifndef APFS_H
#define APFS_H

#include <stdint.h>

typedef uint64_t oid_t;
typedef uint64_t xid_t;
typedef uint64_t paddr_t;

#define APFS_MAGIC               0x4253584E /* 'NXSB' - Container Superblock Magic */
#define APFS_VALID_FS_MAGIC      0x42535041 /* 'APSB' - Volume Superblock Magic */

struct obj_phys_t {
    uint32_t    o_cksum;
    oid_t       o_id;
    xid_t       o_xid;
    uint32_t    o_type;
    uint32_t    o_subtype;
} __attribute__((__packed__));
typedef struct obj_phys_t obj_phys_t;

struct nx_superblock_t {
    obj_phys_t  nx_o;
    uint32_t    nx_magic;
    uint32_t    nx_block_size;
    uint64_t    nx_block_count;
    uint64_t    nx_features;
    uint64_t    nx_readonly_features;
    uint64_t    nx_incompatible_features;
    uuid_t      nx_uuid;
    paddr_t     nx_next_oid;
    paddr_t     nx_next_xid;
    uint32_t    nx_xp_desc_blocks;
    uint32_t    nx_xp_data_blocks;
    paddr_t     nx_xp_desc_base;
    paddr_t     nx_xp_data_base;
    uint32_t    nx_xp_desc_next;
    uint32_t    nx_xp_data_next;
    uint32_t    nx_xp_desc_index;
    uint32_t    nx_xp_data_index;
    xid_t       nx_xp_desc_len;
    xid_t       nx_xp_data_len;
    paddr_t     nx_spaceman_oid;
    paddr_t     nx_omap_oid;
    paddr_t     nx_reaper_oid;
    uint32_t    nx_test_type;
    uint32_t    nx_max_file_systems;
    oid_t       nx_fs_oid[100];
    uint64_t    nx_counters[32];
    paddr_t     nx_blocked_out_prange_oid;
    paddr_t     nx_evict_mapping_tree_oid;
    uint64_t    nx_flags;
    paddr_t     nx_efi_jumpstart;
    uuid_t      nx_fusion_uuid;
    paddr_t     nx_fusion_ms_oid;
    paddr_t     nx_fusion_wbc_oid;
    uint32_t    nx_fusion_wbc_blks;
    uint32_t    pad32;
    oci_t       nx_meta_crypto_id;
    paddr_t     nx_root_tree_oid;
    paddr_t     nx_extent_ref_tree_oid;
    paddr_t     nx_snap_meta_tree_oid;
} __attribute__((__packed__));
typedef struct nx_superblock_t nx_superblock_t;

struct apfs_superblock_t {
    obj_phys_t  apfs_o;
    uint32_t    apfs_magic;
    uint32_t    apfs_fs_index;
    uint64_t    apfs_features;
    uint64_t    apfs_readonly_features;
    uint64_t    apfs_incompatible_features;
    uint64_t    apfs_unmount_txid;
    uint32_t    apfs_fs_flags;
    uint32_t    apfs_block_size;
    uint64_t    apfs_block_count;
    uint64_t    apfs_files_count;
    uint64_t    apfs_folders_count;
    uint64_t    apfs_symlinks_count;
    uint64_t    apfs_other_fs_objects_count;
    uint64_t    apfs_total_blocks_alloced;
    uint64_t    apfs_total_blocks_freed;
    uuid_t      apfs_uuid;
    paddr_t     apfs_next_obj_id;
    paddr_t     apfs_stat_fs_id;
    paddr_t     apfs_root_tree_oid;
    paddr_t     apfs_extent_ref_tree_oid;
    paddr_t     apfs_snap_meta_tree_oid;
    paddr_t     apfs_omap_oid;
    paddr_t     apfs_reaper_oid;
    paddr_t     apfs_quota_oid;
    paddr_t     apfs_ospace_oid;
    paddr_t     apfs_installer_oid;
    uint64_t    apfs_encrypted;
    uint64_t    apfs_major_version;
    uint64_t    apfs_minor_version;
    uint64_t    apfs_evaluation_version;
    uint64_t    apfs_role;
    char        apfs_name[256];
} __attribute__((__packed__));
typedef struct apfs_superblock_t apfs_superblock_t;

struct omap_phys_t {
    obj_phys_t  om_o;
    uint32_t    om_flags;
    uint32_t    om_snap_count;
    paddr_t     om_tree_oid;
    paddr_t     om_snapshot_list_oid;
    paddr_t     om_free_blocks_oid;
    paddr_t     om_alloc_count_oid;
    xid_t       om_pending_revert_min;
    xid_t       om_pending_revert_max;
} __attribute__((__packed__));
typedef struct omap_phys_t omap_phys_t;

struct btree_info_fix_t {
    uint32_t    bt_flags;
    uint32_t    bt_node_size;
    uint32_t    bt_key_count;
    uint32_t    bt_level_count;
} __attribute__((__packed__));
typedef struct btree_info_fix_t btree_info_fix_t;

struct btree_node_phys_t {
    obj_phys_t  btn_o;
    uint16_t    btn_flags;
    uint16_t    btn_level;
    uint32_t    btn_nkeys;
    uint8_t     btn_data[0];
} __attribute__((__packed__));
typedef struct btree_node_phys_t btree_node_phys_t;

#define APFS_OBJ_TYPE_NX_SUPERBLOCK     0x01
#define APFS_OBJ_TYPE_BTREE             0x02
#define APFS_OBJ_TYPE_BTREE_NODE        0x03
#define APFS_OBJ_TYPE_SPACEMAN          0x05
#define APFS_OBJ_TYPE_OMAP              0x07
#define APFS_OBJ_TYPE_FS_SUPERBLOCK     0x09

#endif /* APFS_H */
