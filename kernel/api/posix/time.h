/**
 * @file time.h
 * @author Krisna Pranav
 * @brief 
 * @version 6.0
 * @date 2024-09-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCKS_PER_SEC 1000

struct timespec 
{
    time_t tv_sec;
    long tv_nsec;
}; // struct timespec

typedef int clockid_t;

enum 
{
    CLOCK_REALTIME,
#define CLOCK_REALTIME CLOCK_REALTIME
    CLOCK_MONOTONIC,
#define CLOCK_MONOTONIC CLOCK_MONOTONIC
    CLOCK_MONOTONIC_RAW,
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC_RAW
    CLOCK_REALTIME_COARSE,
#define CLOCK_REALTIME_COARSE CLOCK_REALTIME_COARSE
    CLOCK_MONOTONIC_COARSE,
#define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC_COARSE
    CLOCK_ID_COUNT,
};

#define TIMER_ABSTIME 99

#ifdef __cplusplus
}
#endif
