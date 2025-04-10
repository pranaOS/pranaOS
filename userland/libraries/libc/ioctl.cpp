/**
 * @file ioctl.cpp
 * @author Krisna Pranav
 * @brief ioctl
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <syscall.h>

extern "C" {

/**
 * @param fd 
 * @param request 
 * @param ... 
 * @return int 
 */
int ioctl(int fd, unsigned request, ...)
{
    va_list ap;
    va_start(ap, request);
    FlatPtr arg = va_arg(ap, FlatPtr);
    int rc = syscall(SC_ioctl, fd, request, arg);
    va_end(ap);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
