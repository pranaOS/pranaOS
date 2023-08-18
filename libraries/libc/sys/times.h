/**
 * @file times.h
 * @author Krisna Pranav
 * @brief Times
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/// @brief: tms
struct tms 
{
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
};

/**
 * @return clock_t 
 */
clock_t times(struct tms*);

__END_DECLS
