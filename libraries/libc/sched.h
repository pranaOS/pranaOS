/**
 * @file sched.h
 * @author Krisna Pranav
 * @brief sched
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int sched_yield(void);

struct sched_param {
    int sched_priority;
}; // struct sched_param

#define SCHED_FIFO 0
#define SCHED_RR 1
#define SCHED_OTHER 2
#define SCHED_BATCH 3

/**
 * @param policy 
 * @return int 
 */
int sched_get_priority_min(int policy);

/**
 * @param policy 
 * @return int 
 */
int sched_get_priority_max(int policy);

/**
 * @param pid 
 * @param param 
 * @return int 
 */
int sched_setparam(pid_t pid, const struct sched_param* param);

/**
 * @param pid 
 * @param param 
 * @return int 
 */
int sched_getparam(pid_t pid, struct sched_param* param);

__END_DECLS
