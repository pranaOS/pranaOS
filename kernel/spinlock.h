/**
 * @file spinlock.h
 * @author Krisna Pranav
 * @brief spinlock
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/atomic.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/forward.h>
#include <mods/noncopyable.h>

namespace Kernel {

    template<typename BaseType = u32>
    class SpinLock {
        MOD_MAKE_NONCOPYABLE(SpinLock);
        MOD_MAKE_NONMOVABLE(SpinLock);

    public:
        SpinLock() = default;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 lock() {
            u32 prev_flags;
            Processor::current().enter_critical(prev_flags);
            while (m_lock.exchange(1, Mods::memory_order_acquire) != 0) {
                Processor::wait_check();
            }
            return prev_flags;
        }

        /**
         * @param prev_flags 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock(u32 prev_flags) {
            ASSERT(is_locked());
            m_lock.store(0, Mods::memory_order_release);
            Processor::current().leave_critical(prev_flags);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_locked() const {
            return m_lock.load(Mods::memory_order_relaxed) != 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void initialize() {
            m_lock.store(0, Mods::memory_order_relaxed);
        }

    private:
        Mods::Atomic<BaseType> m_lock { 0 };
    };

    class RecursiveSpinLock {
        MOD_MAKE_NONCOPYABLE(RecursiveSpinLock);
        MOD_MAKE_NONMOVABLE(RecursiveSpinLock);

    public:
        RecursiveSpinLock() = default;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 lock() {
            auto& proc = Processor::current();
            FlatPtr cpu = FlatPtr(&proc);
            u32 prev_flags;
            proc.enter_critical(prev_flags);
            FlatPtr expected = 0;
            while (!m_lock.compare_exchange_strong(expected, cpu, Mods::memory_order_acq_rel)) {
                if (expected == cpu)
                    break;
                Processor::wait_check();
                expected = 0;
            }
            m_recursions++;
            return prev_flags;
        }
        
        /**
         * @param prev_flags 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock(u32 prev_flags) {
            ASSERT(m_recursions > 0);
            ASSERT(m_lock.load(Mods::memory_order_relaxed) == FlatPtr(&Processor::current()));
            if (--m_recursions == 0)
                m_lock.store(0, Mods::memory_order_release);
            Processor::current().leave_critical(prev_flags);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_locked() const {
            return m_lock.load(Mods::memory_order_relaxed) != 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool own_lock() const {
            return m_lock.load(Mods::memory_order_relaxed) == FlatPtr(&Processor::current());
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void initialize() {
            m_lock.store(0, Mods::memory_order_relaxed);
        }

    private:
        Mods::Atomic<FlatPtr> m_lock { 0 };
        u32 m_recursions { 0 };
    };

    template<typename LockType>
    class ScopedSpinLock {
        MOD_MAKE_NONCOPYABLE(ScopedSpinLock);

    public:
        ScopedSpinLock() = delete;
        ScopedSpinLock& operator=(ScopedSpinLock&&) = delete;

        /**
         * @param lock 
         */
        ScopedSpinLock(LockType& lock) : m_lock(&lock) {
            ASSERT(m_lock);
            m_prev_flags = m_lock->lock();
            m_have_lock = true;
        }

        /**
         * @param from 
         */
        ScopedSpinLock(ScopedSpinLock&& from)
            : m_lock(from.m_lock)
            , m_prev_flags(from.m_prev_flags)
            , m_have_lock(from.m_have_lock)
        {
            from.m_lock = nullptr;
            from.m_prev_flags = 0;
            from.m_have_lock = false;
        }

        /**
         * @brief Destroy the Scoped Spin Lock object
         * 
         */
        ~ScopedSpinLock() {
            if (m_lock && m_have_lock) {
                m_lock->unlock(m_prev_flags);
            }
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void lock() {
            ASSERT(m_lock);
            ASSERT(!m_have_lock);
            m_prev_flags = m_lock->lock();
            m_have_lock = true;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock() {
            ASSERT(m_lock);
            ASSERT(m_have_lock);
            m_lock->unlock(m_prev_flags);
            m_prev_flags = 0;
            m_have_lock = false;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool have_lock() const {
            return m_have_lock;
        }

    private:
        LockType* m_lock { nullptr };
        u32 m_prev_flags { 0 };
        bool m_have_lock { false };
    };

}
