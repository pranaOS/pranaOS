/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>
#include <stddef.h>
#include <stdint.h>

__BEGIN_HEADER

typedef int mode_t;

struct stat
{
    uint16_t st_dev;
    uint16_t st_ino;
    uint32_t st_mode;
    uint16_t st_nlink;
    uint16_t st_uid;
    uint16_t st_gid;
    uint16_t st_rdev;
    int32_t st_size;
    uint32_t st_atime;
    uint32_t UNUSED1;
    int32_t st_mtime;
    uint32_t UNUSED2;
    uint32_t st_ctime;
    uint32_t UNUSED3;
    uint32_t st_blksize;
    uint32_t st_blocks;
};

#define _IFMT 0170000   
#define _IFDIR 0040000  
#define _IFCHR 0020000  
#define _IFBLK 0060000  
#define _IFREG 0100000  
#define _IFLNK 0120000  
#define _IFSOCK 0140000 
#define _IFIFO 0010000  

#define S_BLKSIZE 1024

#define S_ISUID 0004000  
#define S_ISGID 0002000  
#define S_ISVTX 0001000  
#define S_IREAD 0000400  
#define S_IWRITE 0000200 
#define S_IEXEC 0000100  
#define S_ENFMT 0002000  

#define S_IFMT _IFMT
#define S_IFDIR _IFDIR
#define S_IFCHR _IFCHR
#define S_IFBLK _IFBLK
#define S_IFREG _IFREG
#define S_IFLNK _IFLNK
#define S_IFSOCK _IFSOCK
#define S_IFIFO _IFIFO

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#define S_IRUSR 0000400 
#define S_IWUSR 0000200 
#define S_IXUSR 0000100 
#define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#define S_IRGRP 0000040 
#define S_IWGRP 0000020 
#define S_IXGRP 0000010 
#define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#define S_IROTH 0000004 
#define S_IWOTH 0000002 
#define S_IXOTH 0000001 

#define S_ISBLK(m) (((m)&_IFMT) == _IFBLK)
#define S_ISCHR(m) (((m)&_IFMT) == _IFCHR)
#define S_ISDIR(m) (((m)&_IFMT) == _IFDIR)
#define S_ISFIFO(m) (((m)&_IFMT) == _IFIFO)
#define S_ISREG(m) (((m)&_IFMT) == _IFREG)
#define S_ISLNK(m) (((m)&_IFMT) == _IFLNK)
#define S_ISSOCK(m) (((m)&_IFMT) == _IFSOCK)

int stat(const char *file, struct stat *st);
int lstat(const char *path, struct stat *st);
int fstat(int fd, struct stat *st);
int mkdir(const char *pathname, mode_t mode);
mode_t umask(mode_t mask);

__END_HEADER