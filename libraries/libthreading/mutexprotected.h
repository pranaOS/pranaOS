/**
 * @file mutexprotected.h
 * @author Krisna Pranav
 * @brief Mutex Protected
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/noncopyable.h>
#include <libthreading/mutex.h>

namespace Threading 
{
    /**
     * @tparam T 
     */
    template<typename T>
    class MutexProtected 
    {
        MOD_MAKE_NONCOPYABLE(MutexProtected);
        MOD_MAKE_NONMOVABLE(MutexProtected);
        using ProtectedType = T;

    public:
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE MutexProtected() = default;

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE MutexProtected(T&& value)
            : m_value(move(value))
        {
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit MutexProtected(T& value)
            : m_value(value)
        {
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @return decltype(auto) 
         */
        template<typename Callback>
        decltype(auto) with_locked(Callback callback)
        {
            auto lock = this->lock();
            return callback(m_value);
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<VoidFunction<T> Callback>
        void for_each_locked(Callback callback)
        {
            with_locked([&](auto& value) {
                for (auto& item : value)
                    callback(item);
            });
        }

    private:
        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE MutexLocker lock() 
        { 
            return MutexLocker(m_lock); 
        }

        T m_value;
        Mutex m_lock {};
    }; // class MutexProtected
} // namespace Threading
