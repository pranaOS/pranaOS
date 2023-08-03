/**
 * @file sched.cpp
 * @author Krisna Pranav
 * @brief sched source
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#include "sched.h"
#include <errno.h>
#include <sched.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @return int 
     */
    int sched_yield()
    {
        int rc = syscall(SC_yield);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param policy 
     * @return int 
     */
    int sched_get_priority_min(int policy)
    {
        (void)policy;
        return 0;
    }

    /**
     * @param policy 
     * @return int 
     */
    int sched_get_priority_max(int policy)
    {
        (void)policy;
        return 3;
    }

    /** 
     * @param pid 
     * @param param 
     * @return int 
     */
    int sched_setparam(pid_t pid, const struct sched_param *param)
    {
        int rc = syscall(SC_sched_setparam, pid, param);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }
}