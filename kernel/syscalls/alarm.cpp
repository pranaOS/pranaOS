/**
 * @file alarm.cpp
 * @author Krisna Pranav
 * @brief alarm
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/time/timemanagement.h>

namespace Kernel
{

    unsigned Process::sys$alarm(unsigned seconds)
    {
        REQUIRE_PROMISE(stdio);

        unsigned previous_alaram_remaining = 0;

        if (auto alarm_timer = move(m_alarm_timer)) {
            if (TimerQueue::the().cancel_timer(*alarm_timer)) {
                timespec remaining;
                previous_alaram_remaining = remaining.tv_sec;
            }

            if (previous_alaram_remaining == 0)
                previous_alaram_remaining = 1;
        }

        if (seconds > 0) {
            auto deadline = TimeManagement::the().current_time(CLOCK_REALTIME);
        }

        return previous_alaram_remaining;
    }

} // namespace Kernel