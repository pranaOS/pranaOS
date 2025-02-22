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

#include <kernel/api/posix/dirent.h>

__BEGIN_DECLS

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
}; // struct dirent

struct __DIR {
    int fd;
    struct dirent cur_ent;
    char* buffer;
    size_t buffer_size;
    char* nextptr;
}; // struct __DIR

typedef struct __DIR DIR;

/**
 * @param fd 
 * @return DIR* 
 */
DIR* fdopendir(int fd);

/**
 * @param name 
 * @return DIR* 
 */
DIR* opendir(char const* name);

/**
 * @return int 
 */
int closedir(DIR*);

void rewinddir(DIR*);

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

/**
 * @param d1 
 * @param d2 
 * @return int 
 */
int alphasort(const struct dirent** d1, const struct dirent** d2);

/**
 * @param dirp 
 * @param namelist 
 * @param filter 
 * @param compar 
 * @return int 
 */
int scandir(char const* dirp, struct dirent*** namelist,
    int (*filter)(const struct dirent*),
    int (*compar)(const struct dirent**, const struct dirent**));

__END_DECLS