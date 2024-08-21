/**
 * @file uio.h
 * @author Krisna Pranav
 * @brief UIO
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/// @brief iovec
struct iovec {
    void* iov_base;
    size_t iov_len;
};

/**
 * @param fd 
 * @param iov_count 
 * @return ssize_t 
 */
ssize_t writev(int fd, const struct iovec*, int iov_count);

__END_DECLS