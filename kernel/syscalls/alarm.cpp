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

    /**
     * @brief sys alaram at process
     * 
     * @param seconds
     */
    unsigned Process::sys$alarm(unsigned seconds)
    {
        REQUIRE_PROMISE(stdio);
        unsigned previous_alarm_remaining = 0;
        if (auto alarm_timer = move(m_alarm_timer)) {
            if (TimerQueue::the().cancel_timer(*alarm_timer)) {
                timespec remaining;
                timespec_add(alarm_timer->remaining(), { 0, 1000000000 - 1 }, remaining);
                previous_alarm_remaining = remaining.tv_sec;
            }

            if (previous_alarm_remaining == 0)
                previous_alarm_remaining = 1;
        }

        if (seconds > 0) {
            auto deadline = TimeManagement::the().current_time(CLOCK_REALTIME).value();

            timespec_add(deadline, { seconds, 0 }, deadline);

            m_alarm_timer = TimerQueue::the().add_timer_without_id(CLOCK_REALTIME, deadline, [this]() {
                (void)send_signal(SIGALRM, nullptr);
            });
        }
        return previous_alarm_remaining;
    }

} // namespace Kernel