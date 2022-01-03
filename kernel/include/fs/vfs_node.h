#pragma once

#include <fs/dirent.h>
#include <fs/fs.h>
#include <sys/types.h>


struct DIR_s;
typedef struct DIR_s DIR;

#include <stddef.h>
#include <stdint.h>

#define VFS_FILE 0x01
#define VFS_READ 0
#define VFS_WRITE 1

struct dirent;
typedef struct vfs_node_s vfs_node_t;