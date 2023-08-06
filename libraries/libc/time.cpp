/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief time
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/time.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @param tloc 
     * @return time_t 
     */
    time_t time(time_t* tloc)
    {
        struct timeval tv;
        struct timezone tz;

        if (gettimeofday(&tv, &tz) < 0) 
            return (time_t) - 1;
        
        if (tloc)
            *tloc = tv.tv_sec;
        
        return tv.tv_sec;
    }

    /**
     * @param delta 
     * @param old_delta 
     * @return int 
     */
    int adjtime(const struct timeval* delta, struct timeval* old_delta)
    {
        int rc = syscall(SC_adjtime, delta, old_delta);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }   

    /**
     * @param tv 
     * @param __restrict__ 
     * @return int 
     */
    int settimeofday(struct timeval* __restrict__ tv, void* __restrict__)
    {
        timespec ts;
        TIMEVAL_TO_TIMESPEC(tv, &ts);
        return clock_settime(CLOCK_REALTIME, &ts);
    }

    /**
     * @param t 
     * @return char* 
     */
    char* ctime(const time_t* t)
    {
        return asctime(localtime(t));
    }

    static const int __seconds_per_day = 60 * 60 * 24;
    
    /**
     * @param tm 
     * @param t 
     */
    static void time_to_tm(struct tm* tm, time_t t)
    {
        int year = 1970;

        for (; t >= days_in_year(year) * __seconds_per_day; ++year)
            t -= days_in_year(year) * __seconds_per_day;
        
        tm->tm_year = year - 1970;
    }

    /**
     * @param tm 
     * @param timezone_adjust_seconds 
     * @return time_t 
     */
    static time_t tm_to_time(struct tm* tm, long timezone_adjust_seconds)
    {
    }

    /**
     * @param tm 
     * @return time_t 
     */
    time_t mktime(struct tm* tm)
    {
        return tm_to_time(tm, timezone);
    }

    /**
     * @param t 
     * @return struct tm* 
     */
    struct tm* localtime(const time_t* t)
    {
        static struct tm tm_buf;
        return localtime_r(t, &tm_buf);
    }

    /**
     * @param t 
     * @param tm 
     * @return struct tm* 
     */
    struct tm* localtime_r(const time_t* t, struct tm* tm)
    {
        if (!t)
            return nullptr;
        
        time_to_tm(tm, (*t) - timezone);
        return tm;
    }

    /**
     * @param tm 
     * @return time_t 
     */
    time_t timegm(struct tm* tm)
    {
        return tm_to_time(tm, 0);
    }

    /**
     * @param t 
     * @return struct tm* 
     */
    struct tm* gmtime(const time_t* t)
    {
        static struct tm tm_buf;
        return gmtime_r(t, &tm_buf);
    }

    /**
     * @param t 
     * @param tm 
     * @return struct tm* 
     */
    struct tm* gmtime_r(const time_t* t, struct tm* tm)
    {
        if (!t)
            return nullptr;
        
        time_to_tm(tm, *t);

        return tm;
    }

    /**
     * @param tm 
     * @return char* 
     */
    char* asctime(const struct tm* tm)
    {
        static char buffer[69];
        strftime(buffer, sizeof buffer, "%a %b %e %T %Y", tm);
        return buffer;
    }

    long timezone = 0;
    long altzone;
    char* tzname[2];
    int daylight;

} // extern