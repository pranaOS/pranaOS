/**
 * @file thread.h
 * @author Krisna Pranav
 * @brief Thread
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/distinctnums.h>
#include <mods/function.h>
#include <mods/result.h>
#include <mods/string.h>
#include <libcore/object.h>
#include <pthread.h>

namespace Threading 
{
    TYPEDEF_DISTINCT_ORDERED_ID(intptr_t, ThreadError);

    class Thread final : public Core::Object 
    {
        C_OBJECT(Thread);

    public:
        /**
         * @brief Destroy the Thread object
         * 
         */
        virtual ~Thread();

        void start();
        void detach();

        /**
         * @tparam T 
         * @return Result<T, ThreadError> 
         */
        template<typename T = void>
        Result<T, ThreadError> join();

        /**
         * @return String 
         */
        String thread_name() const 
        { 
            return m_thread_name; 
        }

        /**
         * @return pthread_t 
         */
        pthread_t tid() const 
        { 
            return m_tid; 
        }

    private:
        /**
         * @brief Construct a new Thread object
         * 
         * @param action 
         * @param thread_name 
         */
        explicit Thread(Function<intptr_t()> action, StringView thread_name = nullptr);

        Function<intptr_t()> m_action;
        pthread_t m_tid { 0 };
        String m_thread_name;
        bool m_detached { false };
    }; // class Thread final : public Core::Object 

    /**
     * @tparam T 
     * @return Result<T, ThreadError> 
     */
    template<typename T>
    Result<T, ThreadError> Thread::join()
    {
        void* thread_return = nullptr;

        int rc = pthread_join(m_tid, &thread_return);

        if (rc != 0) {
            return ThreadError { rc };
        }

        m_tid = 0;
        if constexpr (IsVoid<T>)
            return {};
        else 
            return { static_cast<T>(thread_return) };
    }
} // namespace Threading 