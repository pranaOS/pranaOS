/**
 * @file timer.cpp
 * @author Krisna Pranav
 * @brief timer
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/timer.h>

namespace Core 
{
    /**
     * @param parent 
     */
    Timer::Timer(Object* parent)
        : Object(parent)
    {
    }

    /**
     * @param interval 
     * @param timeout_handler 
     * @param parent 
     */
    Timer::Timer(int interval, Function<void()>&& timeout_handler, Object* parent)
        : Object(parent)
        , on_timeout(move(timeout_handler))
    {
        start(interval);
    }

    /// @brief Destroy the Timer:: Timer object
    Timer::~Timer()
    { }

    /// @breif: timer start
    void Timer::start()
    {
        start(m_interval);
    }

    /**
     * @param interval 
     */
    void Timer::start(int interval)
    {
        if (m_active)
            return;
        m_interval = interval;
        start_timer(interval);
        m_active = true;
    }

    /// @brief timer restart
    void Timer::restart()
    {
        restart(m_interval);
    }

    /**
     * @brief timer restart
     * 
     * @param interval 
     */
    void Timer::restart(int interval)
    {
        if (m_active)
            stop();
        start(interval);
    }

    /// @brief timer stop
    void Timer::stop()
    {
        if (!m_active)
            return;

        stop_timer();
        m_active = false;
    }

    /// @brief: timer event
    void Timer::timer_event(TimerEvent&)
    {
        if (m_single_shot)
            stop();
        else {
            if (m_interval_dirty) {
                stop();
                start(m_interval);
            }
        }

        if (on_timeout)
            on_timeout();
    }
} // namespace Core