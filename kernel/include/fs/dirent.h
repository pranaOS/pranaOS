#pragma once

#include <sys/types.h>

struct filesystem_s;
typedef struct filesystem_s filesystem_t;

struct dirent 
{
    unsigned long d_ino;
    unsigned long d_off;
    unsigned short d_reclen;

    char d_type;
    char d_name[];
};

typedef struct DIR_s
{
    filesystem_t *fs_info;
    offset_t next_direntry_offset;
    ino_t inode;
    blkcnt_t blockpointerindex;
    size_t lastentrysize;
    void * filebuffer;
} DIR;