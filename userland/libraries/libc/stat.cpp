/**
 * @file stat.cpp
 * @author Krisna Pranav
 * @brief stat
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

/**
 * @param mask 
 * @return mode_t 
 */
mode_t umask(mode_t mask)
{
    return syscall(SC_umask, mask);
}

/**
 * @param pathname 
 * @param mode 
 * @return int 
 */
int mkdir(char const* pathname, mode_t mode)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    int rc = syscall(SC_mkdir, pathname, strlen(pathname), mode);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @param mode 
 * @return int 
 */
int chmod(char const* pathname, mode_t mode)
{
    return fchmodat(AT_FDCWD, pathname, mode, 0);
}

/**
 * @param dirfd 
 * @param pathname 
 * @param mode 
 * @param flags 
 * @return int 
 */
int fchmodat(int dirfd, char const* pathname, mode_t mode, int flags)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }

    if (flags & ~AT_SYMLINK_NOFOLLOW) {
        errno = EINVAL;
        return -1;
    }

    Syscall::SC_chmod_params params {
        dirfd,
        { pathname, strlen(pathname) },
        mode,
        !(flags & AT_SYMLINK_NOFOLLOW)
    };
    int rc = syscall(SC_chmod, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param mode 
 * @return int 
 */
int fchmod(int fd, mode_t mode)
{
    int rc = syscall(SC_fchmod, fd, mode);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @param mode 
 * @return int 
 */
int mkfifo(char const* pathname, mode_t mode)
{
    return mknod(pathname, mode | S_IFIFO, 0);
}

/**
 * @param dirfd 
 * @param path 
 * @param statbuf 
 * @param follow_symlinks 
 * @return int 
 */
static int do_stat(int dirfd, char const* path, struct stat* statbuf, bool follow_symlinks)
{
    if (!path) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_stat_params params { { path, strlen(path) }, statbuf, dirfd, follow_symlinks };
    int rc = syscall(SC_stat, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int lstat(char const* path, struct stat* statbuf)
{
    return do_stat(AT_FDCWD, path, statbuf, false);
}

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int stat(char const* path, struct stat* statbuf)
{
    return do_stat(AT_FDCWD, path, statbuf, true);
}

/**
 * @param fd 
 * @param statbuf 
 * @return int 
 */
int fstat(int fd, struct stat* statbuf)
{
    int rc = syscall(SC_fstat, fd, statbuf);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param path 
 * @param statbuf 
 * @param flags 
 * @return int 
 */
int fstatat(int fd, char const* path, struct stat* statbuf, int flags)
{
    return do_stat(fd, path, statbuf, !(flags & AT_SYMLINK_NOFOLLOW));
}
}
