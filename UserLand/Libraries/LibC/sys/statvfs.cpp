/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <errno.h>
#include <string.h>
#include <sys/statvfs.h>
#include <syscall.h>

extern "C" {

int statvfs(const char* path, struct statvfs* buf)
{
    Syscall::SC_statvfs_params params { { path, strlen(path) }, buf };
    int rc = syscall(SC_statvfs, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int fstatvfs(int fd, struct statvfs* buf)
{
    int rc = syscall(SC_fstatvfs, fd, buf);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
