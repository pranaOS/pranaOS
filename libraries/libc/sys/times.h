/**
 * @file times.h
 * @author Krisna Pranav
 * @brief Times
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/times.h>

__BEGIN_DECLS

/**
 * @return clock_t 
 */
clock_t times(struct tms*);

__END_DECLS
