/**
 * @file sched.h
 * @author Krisna Pranav
 * @brief sched
 * @version 6.0
 * @date 2024-09-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sched_param 
{
    int sched_priority;
}; // struct sched_param

#ifdef __cplusplus
}
#endif
