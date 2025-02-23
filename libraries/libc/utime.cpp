/**
 * @file utime.cpp
 * @author Krisna Pranav
 * @brief utime
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <string.h>
#include <syscall.h>
#include <utime.h>

extern "C" {

/**
 * @param pathname 
 * @param buf 
 * @return int 
 */
int utime(char const* pathname, const struct utimbuf* buf)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    int rc = syscall(SC_utime, pathname, strlen(pathname), buf);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
