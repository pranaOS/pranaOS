/**
 * @file uio.h
 * @author Krisna Pranav
 * @brief UIO
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct iovec 
{
    void* iov_base;
    size_t iov_len;
}; // struct iovec

#ifdef __cplusplus
}
#endif
