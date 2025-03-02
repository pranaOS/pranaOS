/**
 * @file elapsedtimer.h
 * @author Krisna Pranav
 * @brief elapsed timer
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/time.h>
#include <sys/time.h>

namespace Core 
{

    class ElapsedTimer 
    {
    public:
        /**
         * @return ElapsedTimer 
         */
        static ElapsedTimer start_new();

        /**
         * @brief Construct a new ElapsedTimer object
         * 
         * @param precise 
         */
        ElapsedTimer(bool precise = false)
            : m_precise(precise)
        {
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_valid; 
        }

        void start();
        void reset();

        /**
         * @return int 
         */
        int elapsed() const;

        /**
         * @return Time 
         */
        Time elapsed_time() const;

        /**
         * @return const struct timeval& 
         */
        const struct timeval& origin_time() const 
        { 
            return m_origin_time; 
        }

    private:
        bool m_precise { false };
        bool m_valid { false };
        struct timeval m_origin_time {
            0, 0
        };
    }; // class ElapsedTimer

} // namespace Core
