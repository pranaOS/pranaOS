/**
 * @file statvfs.cpp
 * @author Krisna Pranav
 * @brief statvfs
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <string.h>
#include <sys/statvfs.h>
#include <syscall.h>

extern "C" {

/**
 * @param path 
 * @param buf 
 * @return int 
 */
int statvfs(char const* path, struct statvfs* buf)
{
    Syscall::SC_statvfs_params params { { path, strlen(path) }, buf };
    int rc = syscall(SC_statvfs, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param buf 
 * @return int 
 */
int fstatvfs(int fd, struct statvfs* buf)
{
    int rc = syscall(SC_fstatvfs, fd, buf);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}
