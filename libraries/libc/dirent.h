/**
 * @file dirent.h
 * @author Krisna Pranav
 * @brief dirent
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

enum {
    DT_UNKNOWN = 0,
#define DT_UNKNOWN DT_UNKNOWN
    DT_FIFO = 1,
#define DT_FIFO DT_FIFO
    DT_CHR = 2,
#define DT_CHR DT_CHR
    DT_DIR = 4,
#define DT_DIR DT_DIR
    DT_BLK = 6,
#define DT_BLK DT_BLK
    DT_REG = 8,
#define DT_REG DT_REG
    DT_LNK = 10,
#define DT_LNK DT_LNK
    DT_SOCK = 12,
#define DT_SOCK DT_SOCK
    DT_WHT = 14
#define DT_WHT DT_WHT
};

/// @brief DIRENT
struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
};

/// @brief DIR
struct __DIR {
    int fd;
    struct dirent cur_ent;
    char* buffer;
    size_t buffer_size;
    char* nextptr;
};

/// @brief __DIR -> DIR
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
