/**
 * @file fcntl.cpp
 * @author Krisna Pranav
 * @brief fcntl
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <syscall.h>

extern "C" {

/**
 * @param fd 
 * @param cmd 
 * @param ... 
 * @return int 
 */
int fcntl(int fd, int cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    u32 extra_arg = va_arg(ap, u32);
    int rc = syscall(SC_fcntl, fd, cmd, extra_arg);
    va_end(ap);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @brief Create a inode watcher object
 * 
 * @param flags 
 * @return int 
 */
int create_inode_watcher(unsigned flags)
{
    int rc = syscall(SC_create_inode_watcher, flags);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param path 
 * @param path_length 
 * @param event_mask 
 * @return int 
 */
int inode_watcher_add_watch(int fd, char const* path, size_t path_length, unsigned event_mask)
{
    Syscall::SC_inode_watcher_add_watch_params params { { path, path_length }, fd, event_mask };
    int rc = syscall(SC_inode_watcher_add_watch, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param wd 
 * @return int 
 */
int inode_watcher_remove_watch(int fd, int wd)
{
    int rc = syscall(SC_inode_watcher_remove_watch, fd, wd);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param mode 
 * @return int 
 */
int creat(char const* path, mode_t mode)
{
    return open(path, O_CREAT | O_WRONLY | O_TRUNC, mode);
}

/**
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int open(char const* path, int options, ...)
{
    if (!path) {
        errno = EFAULT;
        return -1;
    }
    auto path_length = strlen(path);
    if (path_length > INT32_MAX) {
        errno = EINVAL;
        return -1;
    }
    va_list ap;
    va_start(ap, options);
    auto mode = (mode_t)va_arg(ap, unsigned);
    va_end(ap);
    Syscall::SC_open_params params { AT_FDCWD, { path, path_length }, options, mode };
    int rc = syscall(SC_open, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param dirfd 
 * @param path 
 * @param options 
 * @param ... 
 * @return int 
 */
int openat(int dirfd, char const* path, int options, ...)
{
    if (!path) {
        errno = EFAULT;
        return -1;
    }
    auto path_length = strlen(path);
    if (path_length > INT32_MAX) {
        errno = EINVAL;
        return -1;
    }
    va_list ap;
    va_start(ap, options);
    auto mode = (mode_t)va_arg(ap, unsigned);
    va_end(ap);
    Syscall::SC_open_params params { dirfd, { path, path_length }, options, mode };
    int rc = syscall(SC_open, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param offset 
 * @param len 
 * @param advice 
 * @return int 
 */
int posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
    (void)fd;
    (void)offset;
    (void)len;
    (void)advice;
    return 0;
}
}
