/**
 * @file timerqueue.cpp
 * @author Krisna Pranav
 * @brief timer queue
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "timerqueue.h"
#include <mods/function.h>
#include <mods/nonnullownptr.h>
#include <mods/ownptr.h>
#include <mods/singleton.h>
#include <mods/time.h>
#include <kernel/scheduler.h>
#include <kernel/timerqueue.h>
#include <kernel/time/timemanagement.h>

namespace Kernel
{
    
    static Mods::Singleton<TimerQueue> s_the;

    static SpinLock<u8> g_timerqueue_lock;

    /**
     * @return timespec 
     */
    timespec Timer::remaining() const
    {
        if (m_remaining == 0)
            return {};
        
        return TimerQueue::the().ticks_to_time(m_clock_id, m_remaining);
    } 

    u64 Timer::now() const
    {
        return TimerQueue::the().time_to_ticks(m_clock_id, TimeManagement::the().current_time(m_clock_id).value());
    }

    /**
     * @return TimerQueue& 
     */
    TimerQueue& TimerQueue::the()
    {
        return *s_the;
    }

} // namespace Kernel