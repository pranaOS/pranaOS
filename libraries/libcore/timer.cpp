/**
 * @file timer.cpp
 * @author Krisna Pranav
 * @brief timer
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/timer.h>

namespace Core
{

    Timer::Timer(Object* parent)
        : Object(parent)
    {}

    Timer::~Timer()
    {}

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

} // namespace Core