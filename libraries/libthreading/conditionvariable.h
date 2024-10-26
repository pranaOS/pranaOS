/**
 * @file conditionvariable.h
 * @author Krisna Pranav
 * @brief Condition Variable
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <libthreading/mutex.h>
#include <pthread.h>
#include <sys/cdefs.h>
#include <sys/types.h>

namespace Threading 
{
    class ConditionVariable 
    {
        friend class Mutex;

    public:
        /**
         * @brief Construct a new Condition Variable object
         * 
         * @param to_wait_on 
         */
        ConditionVariable(Mutex& to_wait_on)
            : m_to_wait_on(to_wait_on)
        {
            auto result = pthread_cond_init(&m_condition, nullptr);
            VERIFY(result == 0);
        }
        
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~ConditionVariable()
        {
            auto result = pthread_cond_destroy(&m_condition);
            VERIFY(result == 0);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void wait()
        {
            auto result = pthread_cond_wait(&m_condition, &m_to_wait_on.m_mutex);
            VERIFY(result == 0);
        }

        /**
         * @param condition 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void wait_while(Function<bool()> condition)
        {
            while (condition())
                wait();
        }
        
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void signal()
        {
            auto result = pthread_cond_signal(&m_condition);
            VERIFY(result == 0);
        }
        

        ALWAYS_INLINE void broadcast()
        {
            auto result = pthread_cond_broadcast(&m_condition);
            VERIFY(result == 0);
        }

    private:
        pthread_cond_t m_condition;
        Mutex& m_to_wait_on;
    }; // class ConditionVariable
} // namespace Threading
