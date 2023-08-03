/**
 * @file sched.h
 * @author Krisna Pranav
 * @brief sched
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#pragma once 

#include <sched.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int sched_yield();

struct sched_param
{
    int sched_priority;
}; // struct sched_param

#define SCHED_FIFO 0
#define SCHED_RR 1
#define SCHED_OTHER 2

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

__END_DECLS