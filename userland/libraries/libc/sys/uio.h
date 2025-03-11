/**
 * @file uio.h
 * @author Krisna Pranav
 * @brief UIO
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/uio.h>

__BEGIN_DECLS

/**
 * @param fd 
 * @param iov_count 
 * @return ssize_t 
 */
ssize_t writev(int fd, const struct iovec*, int iov_count);

/**
 * @param fd 
 * @param iov_count 
 * @return ssize_t 
 */
ssize_t readv(int fd, const struct iovec*, int iov_count);

__END_DECLS
