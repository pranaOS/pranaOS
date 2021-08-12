/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <syscall.h>

extern "C" {

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
