/**
 * @file stat.h
 * @author Krisna Pranav
 * @brief STAT!!
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/stat.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <time.h>

__BEGIN_DECLS

/**
 * @return mode_t 
 */
mode_t umask(mode_t);

/**
 * @param pathname 
 * @return int 
 */
int chmod(char const* pathname, mode_t);

/**
 * @param fd 
 * @param path 
 * @param mode 
 * @param flag 
 * @return int 
 */
int fchmodat(int fd, char const* path, mode_t mode, int flag);

/**
 * @param fd 
 * @return int 
 */
int fchmod(int fd, mode_t);

/**
 * @param pathname 
 * @return int 
 */
int mkdir(char const* pathname, mode_t);

/**
 * @param pathname 
 * @return int 
 */
int mkfifo(char const* pathname, mode_t);

/**
 * @param fd 
 * @param statbuf 
 * @return int 
 */
int fstat(int fd, struct stat* statbuf);

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int lstat(char const* path, struct stat* statbuf);

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int stat(char const* path, struct stat* statbuf);

/**
 * @param fd 
 * @param path 
 * @param statbuf 
 * @param flags 
 * @return int 
 */
int fstatat(int fd, char const* path, struct stat* statbuf, int flags);

__END_DECLS
