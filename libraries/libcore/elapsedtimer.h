/**
 * @file elapsedtimer.h
 * @author Krisna Pranav
 * @brief elapsed timer
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/time.h>

namespace Core
{
    class ElapsedTimer
    {
    public:
        /// @brief Construct a new Elapsed Timer object
        ElapsedTimer() {}

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        {
            return m_valid;
        }

    private:
        bool m_valid { false };

        struct timval m_origin_time { 0, 0 };
    }; // class ElapsedTimer
} // namespace Core