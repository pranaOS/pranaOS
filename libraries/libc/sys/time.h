/**
 * @file time.h
 * @author Krisna Pranav
 * @brief time
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <time.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct timeval
{
    time_t tv_sec;
    suseconds_t tv_usec;
}; // struct timeval

struct timezone
{
    int tz_minuteswest;
    int tz_dsttime;
}; // struct timezone

/**
 * @param delta 
 * @param old_delta 
 * @return int 
 */
int adjtime(const struct timeval* delta, struct timeval* old_delta);

/**
 * @param a 
 * @param b 
 * @param out 
 */
static inline void timeradd(const struct timeval* a, const struct timeval* b, struct timeval* out)
{
    out->tv_sec = a->tv_sec + b->tv_sec;
    out->tv_usec = a->tv_usec + b ->tv_usec;

    if (out->tv_usec >= 1000 * 1000) {
        out->tv_sec++;
        out->tv_usec -= 1000 * 1000;
    }
} 

/**
 * @param a 
 * @param b 
 * @param out 
 */
static inline void timersub(const struct timeval* a, const struct timeval* b, struct timeval* out)
{
    out->tv_sec = a->tv_sec - b->tv_sec;
    out->tv_sec = a->tv_usec - b->tv_usec;

    if (out->tv_usec < 0) {
        out->tv_usec--;
        out->tv_usec += 1000 * 1000;
    }
}

/**
 * @param out 
 */
static inline void timerclear(struct timeval* out)
{
    out->tv_sec = out->tv_usec = 0;
}

/**
 * @param tv 
 * @return int 
 */
static inline int timerisset(const struct timeval* tv)
{
    return tv->tv_sec || tv->tv_usec;
}

/**
 * @param out 
 */
static inline void timespecclear(struct timespec* out)
{
    out->tv_sec = out->tv_nsec = 0;
}

/**
 * @param ts 
 * @return int 
 */
static inline int timespecisset(const struct timespec* ts)
{
    return ts->tv_sec || ts->tv_nsec;
}

__END_DECLS