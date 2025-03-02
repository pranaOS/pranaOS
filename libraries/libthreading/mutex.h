/**
 * @file mutex.h
 * @author Krisna Pranav
 * @brief mutex
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/format.h>
#include <mods/noncopyable.h>
#include <mods/types.h>
#include <pthread.h>

namespace Threading 
{
    class Mutex 
    {
        MOD_MAKE_NONCOPYABLE(Mutex);
        MOD_MAKE_NONMOVABLE(Mutex);
        friend class ConditionVariable;

    public:
        /**
         * @brief Construct a new Mutex object
         * 
         */
        Mutex()
            : m_lock_count(0)
        {
    #ifndef __pranaos__
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&m_mutex, &attr);
    #endif
        }

        /**
         * @brief Destroy the Mutex object
         * 
         */
        ~Mutex()
        {
            VERIFY(m_lock_count == 0);
        }

        void lock();
        void unlock();

    private:
    #ifdef __pranaos__
        pthread_mutex_t m_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
    #else
        pthread_mutex_t m_mutex;
    #endif
        unsigned m_lock_count { 0 };
    }; // class Mutex 

    class MutexLocker 
    {
        MOD_MAKE_NONCOPYABLE(MutexLocker);
        MOD_MAKE_NONMOVABLE(MutexLocker);

    public:
        /**
         * @param mutex 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit MutexLocker(Mutex& mutex)
            : m_mutex(mutex)
        {
            lock();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~MutexLocker()
        {
            unlock();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock() 
        { 
            m_mutex.unlock(); 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void lock() 
        { 
            m_mutex.lock(); 
        }

    private:
        Mutex& m_mutex;
    }; // class MutexLocker 

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void Mutex::lock()
    {
        pthread_mutex_lock(&m_mutex);
        m_lock_count++;
    }

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void Mutex::unlock()
    {
        VERIFY(m_lock_count > 0);
        m_lock_count--;
        pthread_mutex_unlock(&m_mutex);
    }
} // namespace Threading 
