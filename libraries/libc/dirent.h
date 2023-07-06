/**
 * @file dirent.h
 * @author Krisna Pranav
 * @brief dirent
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

enum {
    DT_UNKNOWN = 0,
    DT_FIFO = 1,
    DT_CHR = 2,
    DT_DIR = 4,
    DT_REG = 8,
};

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
};

struct __DIR {
    int fd;
    struct dirent cur_ent;
    char* buffer;
    size_t buffer_size;
    char* nextptr;
};

typedef struct __DIR DIR;

/**
 * @param name 
 * @return DIR* 
 */
DIR* opendir(const char* name);

/**
 * @return int 
 */
int closedir(DIR*);

/**
 * @return struct dirent* 
 */
struct dirent* readdir(DIR*);

/**
 * @return int 
 */
int readdir_r(DIR*, struct dirent*, struct dirent**);

/**
 * @return int 
 */
int dirfd(DIR*);

__END_DECLS