/**
 * @file time.h
 * @author Krisna Pranav
 * @brief time
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/time.h>
#include <time.h>

__BEGIN_DECLS

struct timezone {
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
 * @param __restrict__ 
 * @param __restrict__ 
 * @return int 
 */
int gettimeofday(struct timeval* __restrict__, void* __restrict__);

/**
 * @param __restrict__ 
 * @param __restrict__ 
 * @return int 
 */
int settimeofday(struct timeval* __restrict__, void* __restrict__);

/**
 * @param pathname 
 * @return int 
 */
int utimes(char const* pathname, const struct timeval[2]);

/**
 * @param a 
 * @param b 
 * @param out 
 */
static inline void timeradd(const struct timeval* a, const struct timeval* b, struct timeval* out)
{
    out->tv_sec = a->tv_sec + b->tv_sec;
    out->tv_usec = a->tv_usec + b->tv_usec;

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
    out->tv_usec = a->tv_usec - b->tv_usec;

    if (out->tv_usec < 0) {
        out->tv_sec--;
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

#define timeradd timeradd
#define timersub timersub
#define timerclear timerclear
#define timerisset timerisset
#define timercmp(tvp, uvp, cmp) \
    (((tvp)->tv_sec == (uvp)->tv_sec) ? ((tvp)->tv_usec cmp(uvp)->tv_usec) : ((tvp)->tv_sec cmp(uvp)->tv_sec))

/**
 * @param a 
 * @param b 
 * @param out 
 */
static inline void timespecadd(const struct timespec* a, const struct timespec* b, struct timespec* out)
{
    out->tv_sec = a->tv_sec + b->tv_sec;
    out->tv_nsec = a->tv_nsec + b->tv_nsec;

    if (out->tv_nsec >= 1000 * 1000 * 1000) {
        out->tv_sec++;
        out->tv_nsec -= 1000 * 1000 * 1000;
    }
}

/**
 * @param a 
 * @param b 
 * @param out 
 */
static inline void timespecsub(const struct timespec* a, const struct timespec* b, struct timespec* out)
{
    out->tv_sec = a->tv_sec - b->tv_sec;
    out->tv_nsec = a->tv_nsec - b->tv_nsec;

    if (out->tv_nsec < 0) {
        out->tv_sec--;
        out->tv_nsec += 1000 * 1000 * 1000;
    }
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

/**
 * @param tv 
 * @param ts 
 */
static inline void TIMEVAL_TO_TIMESPEC(const struct timeval* tv, struct timespec* ts)
{
    ts->tv_sec = tv->tv_sec;
    ts->tv_nsec = tv->tv_usec * 1000;
}

/**
 * @param tv 
 * @param ts 
 */
static inline void TIMESPEC_TO_TIMEVAL(struct timeval* tv, const struct timespec* ts)
{
    tv->tv_sec = ts->tv_sec;
    tv->tv_usec = ts->tv_nsec / 1000;
}

#define timespecadd timespecadd
#define timespecsub timespecsub
#define timespecclear timespecclear
#define timespecisset timespecisset
#define timespeccmp(ts, us, cmp) \
    (((ts)->tv_sec == (us)->tv_sec) ? ((ts)->tv_nsec cmp(us)->tv_nsec) : ((ts)->tv_sec cmp(us)->tv_sec))

__END_DECLS
