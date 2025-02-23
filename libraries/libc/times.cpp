/**
 * @file times.cpp
 * @author Krisna Pranav
 * @brief times
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#include <errno.h>
#include <sys/times.h>
#include <syscall.h>

/**
 * @param buf 
 * @return clock_t 
 */
clock_t times(struct tms* buf)
{
    int rc = syscall(SC_times, buf);
    __RETURN_WITH_ERRNO(rc, rc, (clock_t)-1);
}
