/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct iovec {
    void* iov_base;
    size_t iov_len;
};

ssize_t writev(int fd, const struct iovec*, int iov_count);
ssize_t readv(int fd, const struct iovec*, int iov_count);

__END_DECLS

