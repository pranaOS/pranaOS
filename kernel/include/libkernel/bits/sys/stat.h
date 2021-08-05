/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#ifndef _KERNEL_LIBKERNEL_BITS_SYS_STAT_H
#define _KERNEL_LIBKERNEL_BITS_SYS_STAT_H

#include <libkernel/types.h>

#define S_IFSOCK 0xC000 
#define S_IFLNK 0xA000 
#define S_IFREG 0x8000 
#define S_IFBLK 0x6000 
#define S_IFDIR 0x4000
#define S_IFCHR 0x2000 
#define S_IFIFO 0x1000 

#define S_ISUID 0x0800
#define S_ISGID 0x0400
#define S_ISVTX 0x0200

#define S_IRWXU 0x01c0
#define S_IRUSR 0x0100
#define S_IWUSR 0x0080
#define S_IXUSR 0x0040

#define S_IRWXG 0x0038
#define S_IRGRP 0x0020
#define S_IWGRP 0x0010
#define S_IXGRP 0x0008

#define S_IRWXO 0x0007
#define S_IROTH 0x0004
#define S_IWOTH 0x0002
#define S_IXOTH 0x0001

struct fstat {
    uint32_t dev;
    uint32_t ino;
    mode_t mode; 
    uint32_t nlink; 
    uint32_t uid; 
    uint32_t gid; 
    uint32_t rdev;
    uint32_t size;
    uint32_t atime; 
    uint32_t mtime; 
    uint32_t ctime; 
};

typedef struct fstat fstat_t;

#endif 