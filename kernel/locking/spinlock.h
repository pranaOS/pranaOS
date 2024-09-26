/**
 * @file spinlock.h
 * @author Krisna Pranav
 * @brief Spin Lock
 * @version 6.0
 * @date 2024-09-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/atomic.h>
#include <mods/types.h>
#include <kernel/arch/spinlock.h>
#include <kernel/locking/lockrank.h>

namespace Kernel 
{

    /**
     * @tparam LockType 
     */
    template<typename LockType>
    class [[nodiscard]] SpinlockLocker 
    {
        MOD_MAKE_NONCOPYABLE(SpinlockLocker);

    public:

        /**
         * @brief Construct a new Spinlock Locker object
         * 
         */
        SpinlockLocker() = delete;

        /**
         * @return SpinlockLocker& 
         */
        SpinlockLocker& operator=(SpinlockLocker&&) = delete;

        /**
         * @brief Construct a new Spinlock Locker object
         * 
         * @param lock 
         */
        SpinlockLocker(LockType& lock)
            : m_lock(&lock)
        {
            VERIFY(m_lock);
            m_prev_flags = m_lock->lock();
            m_have_lock = true;
        }

        /**
         * @brief Construct a new Spinlock Locker object
         * 
         * @param from 
         */
        SpinlockLocker(SpinlockLocker&& from)
            : m_lock(from.m_lock)
            , m_prev_flags(from.m_prev_flags)
            , m_have_lock(from.m_have_lock)
        {
            from.m_lock = nullptr;
            from.m_prev_flags = 0;
            from.m_have_lock = false;
        }

        /**
         * @brief Destroy the Spinlock Locker object
         * 
         */
        ~SpinlockLocker()
        {
            if (m_lock && m_have_lock) {
                m_lock->unlock(m_prev_flags);
            }
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void lock()
        {
            VERIFY(m_lock);
            VERIFY(!m_have_lock);
            m_prev_flags = m_lock->lock();
            m_have_lock = true;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock()
        {
            VERIFY(m_lock);
            VERIFY(m_have_lock);
            m_lock->unlock(m_prev_flags);
            m_prev_flags = 0;
            m_have_lock = false;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool have_lock() const
        {
            return m_have_lock;
        }

    private:
        LockType* m_lock { nullptr };
        u32 m_prev_flags { 0 };
        bool m_have_lock { false };
    }; // class [[nodiscard]] SpinlockLocker
} // namespace Kernel