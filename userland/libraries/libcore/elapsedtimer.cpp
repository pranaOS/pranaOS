/**
 * @file elapsedtimer.cpp
 * @author Krisna Pranav
 * @brief elapsed timer
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/time.h>
#include <libcore/elapsedtimer.h>
#include <sys/time.h>
#include <time.h>

namespace Core 
{
    /**
     * @return ElapsedTimer 
     */
    ElapsedTimer ElapsedTimer::start_new()
    {
        ElapsedTimer timer;
        timer.start();
        return timer;
    }

    void ElapsedTimer::start()
    {
        m_valid = true;
        timespec now_spec;

        clock_gettime(m_precise ? CLOCK_MONOTONIC : CLOCK_MONOTONIC_COARSE, &now_spec);

        m_origin_time.tv_sec = now_spec.tv_sec;
        m_origin_time.tv_usec = now_spec.tv_nsec / 1000;
    }

    void ElapsedTimer::reset()
    {
        m_valid = false;
        m_origin_time = { 0, 0 };
    }

    /**
     * @return int 
     */
    int ElapsedTimer::elapsed() const
    {
        VERIFY(is_valid());
        struct timeval now;
        timespec now_spec;

        clock_gettime(m_precise ? CLOCK_MONOTONIC : CLOCK_MONOTONIC_COARSE, &now_spec);

        now.tv_sec = now_spec.tv_sec;
        now.tv_usec = now_spec.tv_nsec / 1000;

        struct timeval diff;

        timeval_sub(now, m_origin_time, diff);

        return diff.tv_sec * 1000 + diff.tv_usec / 1000;
    }

    /**
     * @return Time 
     */
    Time ElapsedTimer::elapsed_time() const
    {
        return Time::from_milliseconds(elapsed());
    }

} // namespace Core
