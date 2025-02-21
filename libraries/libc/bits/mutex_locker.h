/**
 * @file mutex_locker.h
 * @author Krisna Pranav
 * @brief mutex locker
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <pthread.h>

namespace LibC 
{

    class [[nodiscard]] MutexLocker 
    {
    public:
        /**
         * @brief Construct a new MutexLocker object
         * 
         * @param mutex 
         */
        explicit MutexLocker(pthread_mutex_t& mutex)
            : m_mutex(mutex)
        {
            lock();
        }

        /**
         * @brief Destroy the MutexLocker object
         * 
         */
        ~MutexLocker()
        {
            unlock();
        }

        void lock() 
        { 
            pthread_mutex_lock(&m_mutex); 
        }

        void unlock() 
        { 
            pthread_mutex_unlock(&m_mutex); 
        }

    private:
        pthread_mutex_t& m_mutex;
    }; // class [[nodiscard]] MutexLocker

} // namespace LibC
