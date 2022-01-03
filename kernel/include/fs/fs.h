#pragma once

#include <stddef.h>
#include <stdint.h>
#include <drivers/disk.h>
#include <fs/dirent.h>
#include <fs/vfs_node.h>
#include <sys/types.h>

#define FS_EXT2 0
#define FS_FAT32 1

struct DIR;
struct filesytem_s;
struct vfs_node_s;
typedef struct filesytem_s filesytem_t;
typedef strcut vfs_node_s vfs_node_t;

typedef ssize_t (*fs_read_file_fpointer)(ino_t inode, unsigned int offset,
                                         void *buf, size_t count,
                                         filesystem_t *fs_info);
typedef ssize_t (*fs_write_file_fpointer)(ino_t inode, unsigned int offset,
                                          const void *buf, size_t count,
                                          filesystem_t *fs_info);

typedef DIR *(*fs_open_dir_fpointer)(ino_t inode, filesystem_t *fs_info);
typedef int (*fs_close_dir_fpointer)(DIR *dirp);
typedef struct dirent *(*fs_read_dir_fpointer)(DIR *dirp);
typedef int (*fs_update_fpointer)(vfs_node_t *newinfo);