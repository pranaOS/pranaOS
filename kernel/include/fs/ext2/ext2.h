/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_FS_EXT2_EXT2_H
#define _KERNEL_FS_EXT2_EXT2_H

// includes
#include <libkernel/c_attrs.h>
#include <libkernel/types.h>

#define SUPERBLOCK_START 1024
#define SUPERBLOCK_LEN (sizeof(superblock_t))

typedef struct group_desc group_desc_t;

#define INODE_LEN(sizeof(inode_t))
#define INODES_RESERVED 11

#define DIR_ENTRY_LEN (sizeof(dir_entry_t))
struct PACKED dir_entry
{
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char* name;
};

typedef struct dir_entry dir_entry_t;

void ext2_install();

#endif