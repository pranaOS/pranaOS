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
     * @brief Construct a new Timer::Timer object
     * 
     * @param parent 
     */
    Timer::Timer(Object* parent)
        : Object(parent)
    {
    }

    /**
     * @brief Construct a new Timer::Timer object
     * 
     * @param interval_ms 
     * @param timeout_handler 
     * @param parent 
     */
    Timer::Timer(int interval_ms, Function<void()>&& timeout_handler, Object* parent)
        : Object(parent)
        , on_timeout(move(timeout_handler))
    {
        start(interval_ms);
    }

    void Timer::start()
    {
        start(m_interval_ms);
    }

    /**
     * @param interval_ms 
     */
    void Timer::start(int interval_ms)
    {
        if (m_active)
            return;

        m_interval_ms = interval_ms;
        start_timer(interval_ms);
        m_active = true;
    }

    void Timer::restart()
    {
        restart(m_interval_ms);
    }

    /**
     * @param interval_ms 
     */
    void Timer::restart(int interval_ms)
    {
        if (m_active)
            stop();

        start(interval_ms);
    }

    void Timer::stop()
    {
        if (!m_active)
            return;

        stop_timer();
        m_active = false;
    }

    /**
     * @param active 
     */
    void Timer::set_active(bool active)
    {
        if (active)
            start();
        else
            stop();
    }

    void Timer::timer_event(TimerEvent&)
    {
        if (m_single_shot)
            stop();
        else {
            if (m_interval_dirty) {
                stop();
                start(m_interval_ms);
            }
        }

        if (on_timeout)
            on_timeout();
    }

} // namespace Core
