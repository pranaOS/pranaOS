/**
 * @file time.h
 * @author Krisna Pranav
 * @brief Time
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "stdint.h"

__DECL_BEGIN


typedef uint32_t clock_t;
typedef int clockid_t;
typedef int64_t time;

struct timespec {
    time_t tv_sec;
    long tv_usec;
};

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
};  

__DECL_END
