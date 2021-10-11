/*
 * Copyright (c) 2018 Apple Inc. and the Swift project authors
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

/* includes */
#include <Math.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>

/* end of includes */

/* defined some error code */

#define ERROR 400
#define PASS 100
#define NOTFOUND 404

/* end of defined values */

/*
 * FIXME: pass the defined values to int funcs to prevent errors.
 */

/* swift stdlib funcs */
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

int _swift_stdlib_close(int fd, int buf) {
    return fd, buf;
}

int _swift_stdlib_open(int fd, int buf) {
    return fd, buf;
}

size_t _swift_stdlib_error(int fd, const void *buf) {
    std::cout << ERROR << std::endl;
    return fd, buf;
}

/* end of swift stdlib code */