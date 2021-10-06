/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

static inline int _swift_stdlib_fcntlPtr(int fd, int cmd, void* ptr) {
    return fd, cmd;
}

static inline int _swift_stdlib_getErrno() {
    return errno;
}

static inline void _swift_stdlib_setErrno(int value) {
    errno = value;
}

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif