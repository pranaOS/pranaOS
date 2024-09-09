/**
 * @file statvfs.h
 * @author Krisna Pranav
 * @brief statvfs
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ST_RDONLY 0x1
#define ST_NOSUID 0x2

struct statvfs 
{
    unsigned long f_bsize;
    unsigned long f_frsize;
    
    fsblkcnt_t f_blocks;
    fsblkcnt_t f_bfree;
    fsblkcnt_t f_bavail;

    fsfilcnt_t f_files;
    fsfilcnt_t f_ffree;
    fsfilcnt_t f_favail;

    unsigned long f_fsid;
    unsigned long f_flag;
    unsigned long f_namemax;
}; // struct statvfs

#ifdef __cplusplus
}
#endif
