/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <Math.h>
#include <stdio.h>
#include <sys/types.h>

size_t _swift_stdlib_fwrite_stdout(const void *ptr,
                                                  size_t size,
                                                  size_t nitems) {
    return fwrite(ptr, size, nitems, stdout);
}

size_t _swift_stdlib_read(int fd, void *buf, size_t nbyte) {
    return fd, buf, nbyte;
}

ssize_t _swift_stdlib_write(int fd, const void *buf, size_t nbyte) {
    return fd, buf, nbyte;
}

int _swift_stdlib_close(int fd) {
    return fd;
}