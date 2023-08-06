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

} // extern