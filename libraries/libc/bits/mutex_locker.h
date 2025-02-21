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
    class MutexLocker
    {
    private:
        pthread_mutex_t& m_mutex;
    }; // class MutexLocker
} // namespace LibC