/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <fs/vfs.h>
#include <libkern/bits/errno.h>
#include <libkern/libkern.h>
#include <libkern/lock.h>
#include <libkern/log.h>
#include <mem/kmalloc.h>
#include <time/time_manager.h>

#define MAX_BLOCK_LEN 1024

#define SUPERBLOCK _ext2_superblocks[dev->dev->id]
#define GROUPS_COUNT _ext2_group_table_info[dev->dev->id].count
#define GROUP_TABLES _ext2_group_table_info[dev->dev->id].table
#define VFS_DEVICE_LOCK dev->lock
#define VFS_DEVICE_LOCK_OWNED_BY(x) x->dev->lock
#define BLOCK_LEN(sb) (1024 << (sb->log_block_size))
#define TO_EXT_BLOCKS_CNT(sb, x) (x / (2 << (sb->log_block_size)))
#define NORM_FILENAME(x) (x + ((4 - (x & 0b11)) & 0b11))

static superblock_t* _ext2_superblocks[MAX_DEVICES_COUNT];
static groups_info_t _ext2_group_table_info[MAX_DEVICES_COUNT];
static lock_t _ext2_lock;

driver_desc_t _ext2_driver_info();

static void _ext2_read_from_dev(vfs_device_t* dev, uint8_t* buf, uint32_t start, uint32_t len);
static void _ext2_write_to_dev(vfs_device_t* dev, uint8_t* buf, uint32_t start, uint32_t len);
static uint32_t _ext2_get_disk_size(vfs_device_t* dev);

static inline bool _ext2_bitmap_get(uint8_t* bitmap, uint32_t index);
static inline void _ext2_bitmap_set_bit(uint8_t* bitmap, uint32_t index);
static inline void _ext2_bitmap_unset_bit(uint8_t* bitmap, uint32_t index);

static inline uint32_t _ext2_get_group_len(superblock_t* sb);
static inline int _ext2_get_groups_cnt(vfs_device_t* dev, superblock_t* sb);

static uint32_t _ext2_get_block_offset(superblock_t* sb, uint32_t block_index);

static uint32_t _ext2_get_block_of_inode_lev0(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index);
static uint32_t _ext2_get_block_of_inode_lev1(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index);
static uint32_t _ext2_get_block_of_inode_lev2(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index);
static uint32_t _ext2_get_block_of_inode(dentry_t* dentry, uint32_t inode_block_index);

static int _ext2_set_block_of_inode_lev0(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val);
static int _ext2_set_block_of_inode_lev1(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val);
static int _ext2_set_block_of_inode_lev2(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val);
static int _ext2_set_block_of_inode(dentry_t* dentry, uint32_t inode_block_index, uint32_t val);

static int _ext2_find_free_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* block_index, uint32_t group_index);
static int _ext2_allocate_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* block_index, uint32_t pref_group);
static int _ext2_free_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index);

static int _ext2_allocate_block_for_inode(dentry_t* dentry, uint32_t pref_group, uint32_t* block_index);

int ext2_read_inode(dentry_t* dentry);
int ext2_write_inode(dentry_t* dentry);
int ext2_free_inode(dentry_t* dentry);

static int _ext2_find_free_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* inode_index, uint32_t group_index);
static int _ext2_allocate_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* inode_index, uint32_t pref_group);
static int _ext2_free_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t inode_index);
static int _ext2_decriment_links_count(dentry_t* dentry);

static int _ext2_lookup_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, const char* name, uint32_t len, uint32_t* found_inode_index);
static int _ext2_getdirent_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, uint32_t* offset, dirent_t* dirent);
static int _ext2_get_dir_entries_count_in_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index);
static bool _ext2_is_dir_empty(dentry_t* dir);
static int _ext2_add_first_entry_to_dir_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, dentry_t* child_dentry, const char* filename, uint32_t len);
static int _ext2_add_to_dir_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, dentry_t* child_dentry, const char* filename, uint32_t len);
static int _ext2_rm_from_dir_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, dentry_t* child_dentry);

static int _ext2_add_child(dentry_t* dir, dentry_t* child_dentry, const char* name, int len);
static int _ext2_rm_child(dentry_t* dir, dentry_t* child_dentry);
static int _ext2_setup_dir(dentry_t* dir, dentry_t* parent_dir, mode_t mode);

static int _ext2_setup_file(dentry_t* file, mode_t mode);

int ext2_recognize_drive(vfs_device_t* dev);
int ext2_prepare_fs(vfs_device_t* dev);
int ext2_save_state(vfs_device_t* dev);
fsdata_t get_fsdata(dentry_t* dentry);

int ext2_read(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len);
int ext2_write(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len);
int ext2_truncate(dentry_t* dentry, uint32_t len);
int ext2_lookup(dentry_t* dir, const char* name, uint32_t len, dentry_t** result);
int ext2_mkdir(dentry_t* dir, const char* name, uint32_t len, mode_t mode);
int ext2_getdirent(dentry_t* dir, uint32_t* offset, dirent_t* res);
int ext2_create(dentry_t* dir, const char* name, uint32_t len, mode_t mode);
int ext2_rm(dentry_t* dentry);


static void _ext2_read_from_dev(vfs_device_t* dev, uint8_t* buf, uint32_t start, uint32_t len)
{
    void (*read)(device_t * d, uint32_t s, uint8_t * r) = drivers[dev->dev->driver_id].desc.functions[DRIVER_STORAGE_READ];
    int already_read = 0;
    uint32_t sector = start / 512;
    uint32_t start_offset = start % 512;
    uint8_t tmp_buf[512];

    while (len) {
        read(dev->dev, sector, tmp_buf);
        for (int i = 0; i < min(512 - start_offset, len); i++) {
            buf[already_read++] = tmp_buf[start_offset + i];
        }
        len -= min(512 - start_offset, len);
        sector++;
        start_offset = 0;
    }
}

static void _ext2_write_to_dev(vfs_device_t* dev, uint8_t* buf, uint32_t start, uint32_t len)
{
    void (*read)(device_t * d, uint32_t s, uint8_t * r) = drivers[dev->dev->driver_id].desc.functions[DRIVER_STORAGE_READ];
    void (*write)(device_t * d, uint32_t s, uint8_t * r, uint32_t siz) = drivers[dev->dev->driver_id].desc.functions[DRIVER_STORAGE_WRITE];
    int already_written = 0;
    uint32_t sector = start / 512;
    uint32_t start_offset = start % 512;
    uint8_t tmp_buf[512];
    while (len != 0) {
        if (start_offset != 0 || len < 512) {
            read(dev->dev, sector, tmp_buf);
        }
        for (int i = 0; i < min(512 - start_offset, len); i++) {
            tmp_buf[start_offset + i] = buf[already_written++];
        }
        write(dev->dev, sector, tmp_buf, 512);
        len -= min(512 - start_offset, len);
        sector++;
        start_offset = 0;
    }
}

static uint32_t _ext2_get_disk_size(vfs_device_t* dev)
{
    uint32_t (*get_size)(device_t * d) = drivers[dev->dev->driver_id].desc.functions[DRIVER_STORAGE_CAPACITY];
    return get_size(dev->dev);
}


static inline bool _ext2_bitmap_get(uint8_t* bitmap, uint32_t index)
{
    return (bitmap[index / 8] >> (index % 8)) & 1;
}

static inline void _ext2_bitmap_set_bit(uint8_t* bitmap, uint32_t index)
{
    bitmap[index / 8] |= (uint8_t)(1 << (index % 8));
}

static inline void _ext2_bitmap_unset_bit(uint8_t* bitmap, uint32_t index)
{
    bitmap[index / 8] &= ~(1 << (index % 8));
}

static inline uint32_t _ext2_get_group_len(superblock_t* sb)
{
    return BLOCK_LEN(sb) * BLOCK_LEN(sb) * 8;
}

static inline int _ext2_get_groups_cnt(vfs_device_t* dev, superblock_t* sb)
{
    uint32_t sz = _ext2_get_disk_size(dev) - SUPERBLOCK_START;
    uint32_t ans = sz / _ext2_get_group_len(sb);
    return ans;
}


static uint32_t _ext2_get_block_offset(superblock_t* sb, uint32_t block_index)
{
    return SUPERBLOCK_START + (block_index - 1) * BLOCK_LEN(sb);
}

static uint32_t _ext2_get_block_of_inode_lev0(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index)
{
    uint32_t offset = inode_block_index;
    uint32_t res;
    _ext2_read_from_dev(dentry->dev, (uint8_t*)&res, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return res;
}

static uint32_t _ext2_get_block_of_inode_lev1(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index)
{
    uint32_t lev_contain = BLOCK_LEN(dentry->fsdata.sb) / 4;
    uint32_t offset = inode_block_index / lev_contain;
    uint32_t offset_inner = inode_block_index % lev_contain;
    uint32_t res;
    _ext2_read_from_dev(dentry->dev, (uint8_t*)&res, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return res ? _ext2_get_block_of_inode_lev0(dentry, res, offset_inner) : 0;
}

static uint32_t _ext2_get_block_of_inode_lev2(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index)
{
    uint32_t block_len = BLOCK_LEN(dentry->fsdata.sb) / 4;
    uint32_t lev_contain = block_len * block_len;
    uint32_t offset = inode_block_index / lev_contain;
    uint32_t offset_inner = inode_block_index % lev_contain;
    uint32_t res;
    _ext2_read_from_dev(dentry->dev, (uint8_t*)&res, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return res ? _ext2_get_block_of_inode_lev1(dentry, res, offset_inner) : 0;
}

static uint32_t _ext2_get_block_of_inode(dentry_t* dentry, uint32_t inode_block_index)
{
    uint32_t block_len = BLOCK_LEN(dentry->fsdata.sb) / 4;
    if (inode_block_index < 12) {
        return dentry->inode->block[inode_block_index];
    }
    if (inode_block_index < 12 + block_len) { 
        return _ext2_get_block_of_inode_lev0(dentry, dentry->inode->block[12], inode_block_index - 12);
    }
    if (inode_block_index < 12 + block_len + block_len * block_len) { 
        return _ext2_get_block_of_inode_lev1(dentry, dentry->inode->block[13], inode_block_index - 12 - block_len);
    } 
    return _ext2_get_block_of_inode_lev2(dentry, dentry->inode->block[14], inode_block_index - (12 + block_len + block_len * block_len));
}

static int _ext2_set_block_of_inode_lev0(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val)
{
    uint32_t offset = inode_block_index;
    _ext2_write_to_dev(dentry->dev, (uint8_t*)&val, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return 0;
}

static int _ext2_set_block_of_inode_lev1(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val)
{
    uint32_t lev_contain = BLOCK_LEN(dentry->fsdata.sb) / 4;
    uint32_t offset = inode_block_index / lev_contain;
    uint32_t offset_inner = inode_block_index % lev_contain;
    uint32_t res;
    _ext2_read_from_dev(dentry->dev, (uint8_t*)&res, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return res ? _ext2_set_block_of_inode_lev0(dentry, res, offset_inner, val) : -1;
}

static int _ext2_set_block_of_inode_lev2(dentry_t* dentry, uint32_t cur_block, uint32_t inode_block_index, uint32_t val)
{
    uint32_t block_len = BLOCK_LEN(dentry->fsdata.sb) / 4;
    uint32_t lev_contain = block_len * block_len;
    uint32_t offset = inode_block_index / lev_contain;
    uint32_t offset_inner = inode_block_index % lev_contain;
    uint32_t res;
    _ext2_read_from_dev(dentry->dev, (uint8_t*)&res, _ext2_get_block_offset(dentry->fsdata.sb, cur_block) + offset * 4, 4);
    return res ? _ext2_set_block_of_inode_lev1(dentry, res, offset_inner, val) : -1;
}

int _ext2_set_block_of_inode(dentry_t* dentry, uint32_t inode_block_index, uint32_t val)
{
    uint32_t block_len = BLOCK_LEN(dentry->fsdata.sb) / 4;
    if (inode_block_index < 12) {
        dentry->inode->block[inode_block_index] = val;
        dentry_set_flag(dentry, DENTRY_DIRTY);
        return 0;
    }
    if (inode_block_index < 12 + block_len) { 
        return _ext2_set_block_of_inode_lev0(dentry, dentry->inode->block[12], inode_block_index - 12, val);
    }
    if (inode_block_index < 12 + block_len + block_len * block_len) { 
        return _ext2_set_block_of_inode_lev1(dentry, dentry->inode->block[13], inode_block_index - 12 - block_len, val);
    } 
    return _ext2_set_block_of_inode_lev2(dentry, dentry->inode->block[14], inode_block_index - (12 + block_len + block_len * block_len), val);
}

static int _ext2_find_free_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* block_index, uint32_t group_index)
{
    uint8_t block_bitmap[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, block_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].block_bitmap), BLOCK_LEN(fsdata.sb));

    for (uint32_t off = 0; off < 8 * BLOCK_LEN(fsdata.sb); off++) {
        if (!_ext2_bitmap_get(block_bitmap, off)) {
            *block_index = fsdata.sb->blocks_per_group * group_index + off + 1;
            _ext2_bitmap_set_bit(block_bitmap, off);
            _ext2_write_to_dev(dev, block_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].block_bitmap), BLOCK_LEN(fsdata.sb));
            return 0;
        }
    }
    return -ENOSPC;
}

static int _ext2_allocate_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* block_index, uint32_t pref_group)
{
    uint32_t groups_cnt = GROUPS_COUNT;
    for (int i = 0; i < groups_cnt; i++) {
        uint32_t group_id = (pref_group + i) % groups_cnt;
        if (GROUP_TABLES[group_id].free_blocks_count) {
            if (_ext2_find_free_block_index(dev, fsdata, block_index, group_id) == 0) {
                return 0;
            }
        }
    }
    return -ENOSPC;
}

static int _ext2_free_block_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index)
{
    block_index--;
    uint32_t block_len = BLOCK_LEN(fsdata.sb);
    uint32_t group_index = block_index / block_len;
    uint32_t off = block_index % block_len;

    uint8_t block_bitmap[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, block_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].block_bitmap), block_len);

    _ext2_bitmap_unset_bit(block_bitmap, off);
    _ext2_write_to_dev(dev, block_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].block_bitmap), block_len);
    return 0;
}

static int _ext2_allocate_block_for_inode(dentry_t* dentry, uint32_t pref_group, uint32_t* block_index)
{
    if (_ext2_allocate_block_index(dentry->dev, dentry->fsdata, block_index, pref_group) == 0) {
        uint32_t blocks_per_inode = TO_EXT_BLOCKS_CNT(dentry->fsdata.sb, dentry->inode->blocks);
        if (_ext2_set_block_of_inode(dentry, blocks_per_inode, *block_index) == 0) {
            dentry->inode->blocks += BLOCK_LEN(dentry->fsdata.sb) / 512;
            dentry_set_flag(dentry, DENTRY_DIRTY);
            return 0;
        }
    }
    return -ENOSPC;
}

int ext2_read_inode(dentry_t* dentry)
{
    uint32_t inodes_per_group = dentry->fsdata.sb->inodes_per_group;
    uint32_t holder_group = (dentry->inode_indx - 1) / inodes_per_group;
    uint32_t pos_inside_group = (dentry->inode_indx - 1) % inodes_per_group;
    uint32_t inode_start = _ext2_get_block_offset(dentry->fsdata.sb, dentry->fsdata.gt->table[holder_group].inode_table) + (pos_inside_group * INODE_LEN);
    _ext2_read_from_dev(dentry->dev, (uint8_t*)dentry->inode, inode_start, INODE_LEN);
    return 0;
}

int ext2_write_inode(dentry_t* dentry)
{
    uint32_t inodes_per_group = dentry->fsdata.sb->inodes_per_group;
    uint32_t holder_group = (dentry->inode_indx - 1) / inodes_per_group;
    uint32_t pos_inside_group = (dentry->inode_indx - 1) % inodes_per_group;
    uint32_t inode_start = _ext2_get_block_offset(dentry->fsdata.sb, dentry->fsdata.gt->table[holder_group].inode_table) + (pos_inside_group * INODE_LEN);
    _ext2_write_to_dev(dentry->dev, (uint8_t*)dentry->inode, inode_start, INODE_LEN);
    return 0;
}

static int _ext2_find_free_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* inode_index, uint32_t group_index)
{
    uint8_t inode_bitmap[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, inode_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].inode_bitmap), BLOCK_LEN(fsdata.sb));

    for (uint32_t off = 0; off < 8 * BLOCK_LEN(fsdata.sb); off++) {
        if (!_ext2_bitmap_get(inode_bitmap, off)) {
            *inode_index = SUPERBLOCK->inodes_per_group * group_index + off + 1;
            _ext2_bitmap_set_bit(inode_bitmap, off);
            _ext2_write_to_dev(dev, inode_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].inode_bitmap), BLOCK_LEN(fsdata.sb));
            return 0;
        }
    }
    return -ENOSPC;
}

static int _ext2_allocate_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t* inode_index, uint32_t pref_group)
{
    uint32_t groups_cnt = GROUPS_COUNT;
    for (int i = 0; i < groups_cnt; i++) {
        uint32_t group_id = (pref_group + i) % groups_cnt;
        if (fsdata.gt->table[group_id].free_inodes_count) {
            if (_ext2_find_free_inode_index(dev, fsdata, inode_index, group_id) == 0) {
                return 0;
            }
        }
    }
    return -ENOSPC;
}

static int _ext2_free_inode_index(vfs_device_t* dev, fsdata_t fsdata, uint32_t inode_index)
{
    inode_index--;
    uint32_t block_len = BLOCK_LEN(fsdata.sb);
    uint32_t inodes_per_group = fsdata.sb->inodes_per_group;
    uint32_t group_index = inode_index / inodes_per_group;
    uint32_t off = inode_index % inodes_per_group;

    uint8_t inode_bitmap[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, inode_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].inode_bitmap), block_len);

    _ext2_bitmap_unset_bit(inode_bitmap, off);
    _ext2_write_to_dev(dev, inode_bitmap, _ext2_get_block_offset(fsdata.sb, fsdata.gt->table[group_index].inode_bitmap), block_len);
    return 0;
}

int ext2_free_inode(dentry_t* dentry)
{
    ASSERT(dentry->d_count == 0 && dentry->inode->links_count == 0);
    uint32_t block_per_dir = TO_EXT_BLOCKS_CNT(dentry->fsdata.sb, dentry->inode->blocks);

    for (int block_index = 0; block_index < block_per_dir; block_index++) {
        uint32_t data_block_index = _ext2_get_block_of_inode(dentry, block_index);
        _ext2_free_block_index(dentry->dev, dentry->fsdata, data_block_index);
    }

    _ext2_free_inode_index(dentry->dev, dentry->fsdata, dentry->inode_indx);
    return 0;
}

static int _ext2_decriment_links_count(dentry_t* dentry)
{
    return (--dentry->inode->links_count) == 0;
}

static int _ext2_lookup_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, const char* name, uint32_t len, uint32_t* found_inode_index)
{
    if (block_index == 0) {
        return -EINVAL;
    }

    uint8_t tmp_buf[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, tmp_buf, _ext2_get_block_offset(fsdata.sb, block_index), BLOCK_LEN(fsdata.sb));
    dir_entry_t* start_of_entry = (dir_entry_t*)tmp_buf;
    for (;;) {
        if (start_of_entry->inode == 0) {
            return -EFAULT;
        }

        if (start_of_entry->name_len == len) {
            bool is_name_same = true;
            for (int i = 0; i < start_of_entry->name_len; i++) {
                is_name_same &= (name[i] == *((char*)start_of_entry + 8 + i));
            }

            if (is_name_same) {
                *found_inode_index = start_of_entry->inode;
                return 0;
            }
        }

        start_of_entry = (dir_entry_t*)((uint32_t)start_of_entry + start_of_entry->rec_len);
        if ((uint32_t)start_of_entry >= (uint32_t)tmp_buf + BLOCK_LEN(fsdata.sb)) {
            return -EFAULT;
        }
    }
    return -EFAULT;
}

static int _ext2_getdirent_block(vfs_device_t* dev, fsdata_t fsdata, uint32_t block_index, uint32_t* offset, dirent_t* dirent)
{
    if (block_index == 0) {
        return -EINVAL;
    }
    const uint32_t block_len = BLOCK_LEN(fsdata.sb);
    uint32_t internal_offset = *offset % block_len;

    uint8_t tmp_buf[MAX_BLOCK_LEN];
    _ext2_read_from_dev(dev, tmp_buf, _ext2_get_block_offset(fsdata.sb, block_index), block_len);
    for (;;) {
        dir_entry_t* start_of_entry = (dir_entry_t*)((uint32_t)tmp_buf + internal_offset);
        internal_offset += start_of_entry->rec_len;
        *offset += start_of_entry->rec_len;

        if (start_of_entry->inode != 0) {
            int name_len = start_of_entry->name_len;
            if (name_len > 251) {
                log_warn("[VFS] Full name len is unsupported\n");
                name_len = 251;
            }
            memcpy(dirent->name, (char*)start_of_entry + 8, name_len);
            dirent->name[name_len] = '\0';
            return 0;
        }

        if (internal_offset >= block_len) {
            return -EFAULT;
        }
    }
    return -EFAULT;
}
