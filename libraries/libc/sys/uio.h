/**
 * @file uio.h
 * @author Krisna Pranav
 * @brief UIO
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct iovec {
    void* iov_base;
    size_t iov_len;
};

ssize_t writev();

__END_DECLS