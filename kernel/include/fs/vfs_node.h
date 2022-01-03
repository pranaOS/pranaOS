#pragma once

#include <fs/dirent.h>
#include <fs/fs.h>
#include <sys/types.h>


struct DIR_s;
typedef struct DIR_s DIR;

#include <stddef.h>
#include <stdint.h>

#define VFS_FILE        0x01
#define VFS_DIRECTORY   0x02
#define VFS_CHARDEVICE  0x03
#define VFS_BLOCKDEVICE 0x04
#define VFS_PIPE        0x05
#define VFS_SYMLINK     0x06

#define VFS_MOUNTPOINT 0x08
#define VFS_SOCKET     0x09

#define VFS_READ  0
#define VFS_WRITE 1

struct dirent;
typedef struct vfs_node_s vfs_node_t;