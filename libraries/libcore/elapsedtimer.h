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
        ///  @brief Construct a new Elapsed Timer object
        ElapsedTimer() { }
        
        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_valid; 
        }

        void start();

        /**
         * @return int 
         */
        int elapsed() const;

        /**
         * @return const struct timeval& 
         */
        const struct timeval& origin_time() const 
        { 
            return m_origin_time; 
        }

    private:
        bool m_valid { false };
        struct timeval m_origin_time { 0, 0 };
    }; // class ElapsedTimer

} // namespace Core