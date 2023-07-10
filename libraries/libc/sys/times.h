/**
 * @file times.h
 * @author Krisna Pranav
 * @brief Times
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/// @brief times
struct tms {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
};

/**
 * @return clock_t 
 */
clock_t times(struct tms*);

__END_DECLS