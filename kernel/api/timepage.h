/**
 * @file timepage.h
 * @author Krisna Pranav
 * @brief time page
 * @version 6.0
 * @date 2024-09-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

#ifdef KERNEL
#    include <kernel/unixtypes.h>
#else
#    include <time.h>
#endif

namespace Kernel 
{
    /**
     * @param clock_id 
     * @return true 
     * @return false 
     */
    inline bool time_page_supports(clockid_t clock_id)
    {
        return clock_id == CLOCK_REALTIME_COARSE || clock_id == CLOCK_MONOTONIC_COARSE;
    }

    struct TimePage 
    {
        volatile u32 update1;
        struct timespec clocks[CLOCK_ID_COUNT];
        volatile u32 update2;
    }; // struct TimePage

} // namespace Kernel