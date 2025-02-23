/**
 * @file fcntl.h
 * @author Krisna Pranav
 * @brief fcntl
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/fcntl.h>

__BEGIN_DECLS

#define POSIX_FADV_DONTNEED 1
#define POSIX_FADV_NOREUSE 2
#define POSIX_FADV_NORMAL 3
#define POSIX_FADV_RANDOM 4
#define POSIX_FADV_SEQUENTIAL 5
#define POSIX_FADV_WILLNEED 6

/**
 * @param path 
 * @return int 
 */
int creat(char const* path, mode_t);

/**
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int open(char const* path, int options, ...);

/**
 * @param dirfd 
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int openat(int dirfd, char const* path, int options, ...);

/**
 * @param fd 
 * @param cmd 
 * @param ... 
 * @return int 
 */
int fcntl(int fd, int cmd, ...);

/**
 * @brief Create a inode watcher object
 * 
 * @param flags 
 * @return int 
 */
int create_inode_watcher(unsigned flags);

/**
 * @param fd 
 * @param path 
 * @param path_length 
 * @param event_mask 
 * @return int 
 */
int inode_watcher_add_watch(int fd, char const* path, size_t path_length, unsigned event_mask);

/**
 * @param fd 
 * @param wd 
 * @return int 
 */
int inode_watcher_remove_watch(int fd, int wd);

/**
 * @param fd 
 * @param offset 
 * @param len 
 * @param advice 
 * @return int 
 */
int posix_fadvise(int fd, off_t offset, off_t len, int advice);

__END_DECLS