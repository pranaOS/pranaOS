/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/types.h>

/**
 * @brief ACCMODE, RDONLY, WRONLY, RDWR, CREAT, TRUNC, EXCL, NOCCTY, NONBLOCK, APPEND, NDELAY, SYNC, FASYNC, DIRECTORY, NOFOLLOW, LRAGEFILE, DIRECT, NOATTIME
 * 
 */
#define O_ACCMODE 0003
#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 01000	
#define O_TRUNC 02000	
#define O_EXCL 04000	
#define O_NOCTTY 010000 
#define O_NONBLOCK 00004
#define O_APPEND 00010
#define O_NDELAY O_NONBLOCK
#define O_SYNC 040000
#define FASYNC 020000		
#define O_DIRECTORY 0100000 
#define O_NOFOLLOW 0200000	
#define O_LARGEFILE 0400000 
#define O_DIRECT 02000000	
#define O_NOATIME 04000000

/**
 * @brief DUPFD, GETFD, SETFD, GETFL, SETFL, GETLK, SETLK, SETOWN, GETOWN, SETSIG, GETSIG, CLOCEXEC
 * 
 */
#define F_DUPFD 0 
#define F_GETFD 1 
#define F_SETFD 2 
#define F_GETFL 3 
#define F_SETFL 4 
#define F_GETLK 5
#define F_SETLK 6
#define F_SETLKW 7
#define F_SETOWN 8
#define F_GETOWN 9
#define F_SETSIG 10
#define F_GETSIG 11
#define FD_CLOEXEC 1

/**
 * @brief FDCWD, EACCESS, SYMLINK_NOFOLLOW, SYMLINK_FOLLOW, REMOVEDIR
 * 
 */
#define AT_FDCWD -2
#define AT_EACCESS 1
#define AT_SYMLINK_NOFOLLOW 2
#define AT_SYMLINK_FOLLOW 4
#define AT_REMOVEDIR 8

/**
 * @brief open, fcntl, create
 * 
 * @param path 
 * @param oflag 
 * @param ... 
 * @return int 
 */
int open(const char* path, int oflag, ...);
int fcntl(int fd, int cmd, ...);
int create(const char* path, mode_t mode);