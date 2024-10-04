/**
 * @file mutexprotected.h
 * @author Krisna Pranav
 * @brief Mutex Protected
 * @version 6.0
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/locking/mutex.h>

namespace Kernel 
{

    /**
     * @tparam T 
     */
    template<typename T>
    class MutexProtected 
    {
        MOD_MAKE_NONCOPYABLE(MutexProtected);
        MOD_MAKE_NONMOVABLE(MutexProtected);

    private:
        /**
         * @tparam U 
         * @tparam lock_mode 
         */
        template<typename U, LockMode lock_mode>
        class Locked 
        {
            MOD_MAKE_NONCOPYABLE(Locked);
            MOD_MAKE_NONMOVABLE(Locked);

        public: 
            /**
             * @brief Construct a new Locked object
             * 
             * @param value 
             * @param mutex 
             * @param location 
             */
            Locked(U& value, Mutex& mutex, LockLocation const& location)
                : m_value(value)
                , m_locker(mutex, lock_mode, location)
            {
            }

            /**
             * @return ALWAYS_INLINE const* 
             */
            ALWAYS_INLINE U const* operator->() const 
            { 
                return &m_value; 
            }

            /**
             * @return ALWAYS_INLINE const& 
             */
            ALWAYS_INLINE U const& operator*() const 
            { 
                return m_value; 
            }

            ALWAYS_INLINE U* operator->() requires(!IsConst<U>) 
            { 
                return &m_value; 
            }

            ALWAYS_INLINE U& operator*() requires(!IsConst<U>) 
            { 
                return m_value; 
            }

            ALWAYS_INLINE U const& get() const 
            { 
                return &m_value; 
            }
            ALWAYS_INLINE U& get() requires(!IsConst<U>) { return &m_value; }

        private:
            U& m_value;
            MutexLocker m_locker;
        }; // class Locked

        /**
         * @param location 
         * @return auto 
         */
        auto lock_shared(LockLocation const& location) const 
        { 
            return Locked<T const, LockMode::Shared>(m_value, m_mutex, location); 
        }

        /**
         * @param location 
         * @return auto 
         */
        auto lock_exclusive(LockLocation const& location) 
        { 
            return Locked<T, LockMode::Exclusive>(m_value, m_mutex, location); 
        }

    public:
        /**
         * @brief Construct a new Mutex Protected object
         * 
         */
        MutexProtected() = default;

        /**
         * @tparam Callback 
         * @param callback 
         * @param location 
         * @return decltype(auto) 
         */
        template<typename Callback>
        decltype(auto) with_shared(Callback callback, LockLocation const& location = LockLocation::current()) const
        {
            auto lock = lock_shared(location);
            return callback(*lock);
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @param location 
         * @return decltype(auto) 
         */
        template<typename Callback>
        decltype(auto) with_exclusive(Callback callback, LockLocation const& location = LockLocation::current())
        {
            auto lock = lock_exclusive(location);
            return callback(*lock);
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @param location 
         */
        template<typename Callback>
        void for_each_shared(Callback callback, LockLocation const& location = LockLocation::current()) const
        {
            with_shared([&](const auto& value) {
                for (auto& item : value)
                    callback(item);
            },
                location);
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @param location 
         */
        template<typename Callback>
        void for_each_exclusive(Callback callback, LockLocation const& location = LockLocation::current())
        {
            with_exclusive([&](auto& value) {
                for (auto& item : value)
                    callback(item);
            },
                location);
        }

    private:
        T m_value;
        Mutex mutable m_mutex;
    }; // class MutexProtected
} // namespace Kernel
