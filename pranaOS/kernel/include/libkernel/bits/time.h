/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_BITS_TIME_H
#define _KERNEL_LIBKERNEL_BITS_TIME_H

#include <libkernel/types.h>

struct timeval {
    time_t tv_sec;
    uint32_t tv_usec;
};
typedef struct timeval timeval_t;

#define DST_NONE 0 
#define DST_USA 1 
#define DST_AUST 2 
#define DST_WET 3 
#define DST_MET 4 
#define DST_EET 5 
#define DST_CAN 6 
#define DST_IND 7

struct timezone {
    int tz_minuteswest; 
    int tz_dsttime; 
};
typedef struct timezone timezone_t;

struct timespec {
    time_t tv_sec;
    uint32_t tv_nsec;
};
typedef struct timespec timespec_t;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};
typedef struct tm tm_t;

typedef enum {
    CLOCK_REALTIME,
    CLOCK_MONOTONIC,
    CLOCK_PROCESS_CPUTIME_ID,
    CLOCK_THREAD_CPUTIME_ID,
} clockid_t;

#endif