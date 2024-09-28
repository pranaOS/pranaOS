/**
 * @file spinlockprotected.h
 * @author Krisna Pranav
 * @brief Spin Lock Protected
 * @version 6.0
 * @date 2024-09-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <kernel/locking/spinlock.h>

namespace Kernel 
{

    template<typename T>
    class SpinlockProtected 
    {
        MOD_MAKE_NONCOPYABLE(SpinlockProtected);
        MOD_MAKE_NONMOVABLE(SpinlockProtected);

    private:
        /**
         * @tparam U 
         */
        template<typename U>
        class Locked 
        {
            MOD_MAKE_NONCOPYABLE(Locked);
            MOD_MAKE_NONMOVABLE(Locked);

        public:

            /**
             * @brief Construct a new Locked object
             * 
             * @param value 
             * @param spinlock 
             */
            Locked(U& value, RecursiveSpinlock& spinlock)
                : m_value(value)
                , m_locker(spinlock)
            {
            }
    
            ALWAYS_INLINE U const* operator->() const 
            { 
                return &m_value; 
            }

            ALWAYS_INLINE U const& operator*() const 
            { 
                return m_value; 
            }

            ALWAYS_INLINE U* operator->() 
            { 
                return &m_value; 
            }

            ALWAYS_INLINE U& operator*() 
            {
                return m_value; 
            }

            ALWAYS_INLINE U const& get() const 
            { 
                return m_value; 
            }

            ALWAYS_INLINE U& get() 
            { 
                return m_value; 
            }

        private:
            U& m_value;
            SpinlockLocker<RecursiveSpinlock> m_locker;
        }; // clas Locked

        auto lock_const() const 
        { 
            return Locked<T const>(m_value, m_spinlock); 
        }

        auto lock_mutable() 
        { 
            return Locked<T>(m_value, m_spinlock); 
        }

    public:
        SpinlockProtected() = default;

        /**
         * @tparam Callback 
         * @param callback 
         * @return decltype(auto) 
         */
        template<typename Callback>
        decltype(auto) with(Callback callback) const
        {
            auto lock = lock_const();
            return callback(*lock);
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @return decltype(auto) 
         */
        template<typename Callback>
        decltype(auto) with(Callback callback)
        {
            auto lock = lock_mutable();
            return callback(*lock);
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void for_each_const(Callback callback) const
        {
            with([&](const auto& value) {
                for (auto& item : value)
                    callback(item);
            });
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void for_each(Callback callback)
        {
            with([&](auto& value) {
                for (auto& item : value)
                    callback(item);
            });
        }

    private:
        T m_value;
        RecursiveSpinlock mutable m_spinlock;
    }; // class SpinlockProtected
}
