/**
 * @file stat.h
 * @author Krisna Pranav
 * @brief stat
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>
#include <kernel/api/posix/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S_IFMT 0170000
#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFBLK 0060000
#define S_IFREG 0100000
#define S_IFIFO 0010000
#define S_IFLNK 0120000
#define S_IFSOCK 0140000

#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC S_IXUSR
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)

#define S_IRWXG (S_IRWXU >> 3)
#define S_IRWXO (S_IRWXG >> 3)
#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#define S_ISCHR(m) (((m)&S_IFMT) == S_IFCHR)
#define S_ISBLK(m) (((m)&S_IFMT) == S_IFBLK)
#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)
#define S_ISLNK(m) (((m)&S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m)&S_IFMT) == S_IFSOCK)

struct stat 
{
    dev_t st_dev;            

    ino_t st_ino;            

    mode_t st_mode;          

    nlink_t st_nlink;        

    uid_t st_uid;            

    gid_t st_gid;            
    
    dev_t st_rdev;           

    off_t st_size;           

    blksize_t st_blksize;    
    blkcnt_t st_blocks;      

    struct timespec st_atim; 
    struct timespec st_mtim; 
    struct timespec st_ctim; 
}; // struct stat

#define st_atime st_atim.tv_sec
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec

#ifdef __cplusplus
}
#endif
