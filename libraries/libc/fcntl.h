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

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define F_DUPFD 0
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3
#define F_SETFL 4
#define F_ISTTY 5
#define FD_CLOEXEC 1

#define O_RDONLY (1 << 0)
#define O_WRONLY (1 << 1)
#define O_RDWR (O_RDONLY | O_WRONLY)
#define O_ACCMODE (O_RDONLY | O_WRONLY)
#define O_EXEC (1 << 2)
#define O_CREAT (1 << 3)
#define O_EXCL (1 << 4)
#define O_NOCTTY (1 << 5)
#define O_TRUNC (1 << 6)
#define O_APPEND (1 << 7)
#define O_NONBLOCK (1 << 8)
#define O_DIRECTORY (1 << 9)
#define O_NOFOLLOW (1 << 10)
#define O_CLOEXEC (1 << 11)
#define O_DIRECT (1 << 12)

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
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)

#define S_IRWXG (S_IRWXU >> 3)
#define S_IRWXO (S_IRWXG >> 3)

/**
 * @param path 
 * @return int 
 */
int creat(const char* path, mode_t);

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
int creat_with_path_length(const char* path, size_t path_length, mode_t);

/**
 * @param path 
 * @param path_length 
 * @param options 
 * @return int 
 */
int open_with_path_length(const char* path, size_t path_length, int options, mode_t);

// AT_FDCWD -
#define AT_FDCWD -100

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
 * @param options 
 * @return int 
 */
int openat_with_path_length(int dirfd, const char* path, size_t path_length, int options, mode_t);

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

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2
#define F_SETLK 6
#define F_SETLKW 7

/// @brief: flock 
struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};

__END_DECLS 