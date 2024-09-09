/**
 * @file time.h
 * @author Krisna Pranav
 * @brief time
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

struct timeval 
{
    time_t tv_sec;
    suseconds_t tv_usec;
}; // struct timeval

#ifdef __cplusplus
}
#endif
