/**
 * @file fcntl.h
 * @author Krisna Pranav
 * @brief fcntl
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

/**
 * @param path 
 * @return int 
 */
int create(const char* path, mode_t);

/**
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int open(const char* path, int options, ...);

/**
 * @param path 
 * @param path_length 
 * @return int 
 */
int create_with_path_length(const char* path, size_t path_length, mode_t);

/**
 * @param path 
 * @param path_length 
 * @param options 
 * @return int 
 */
int open_with_path_length(const char* path, size_t path_length, int options, mode_t);

/**
 * @param dirfd 
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int openat(int dirfd, const char* path, int options, ...);

/**
 * @param dirfd 
 * @param path 
 * @param path_length 
 * @return int 
 */
int openat_with_path_length(int dirfd, const char* path, size_t path_length, mode_t);

/**
 * @param fd 
 * @param cmd 
 * @param ... 
 * @return int 
 */
int fcntl(int fd, int cmd, ...);

/**
 * @param path 
 * @param path_length 
 * @return int 
 */
int watch_file(const char* path, size_t path_length);

struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
};

__END_DECLS