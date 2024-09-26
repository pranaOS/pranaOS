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

#include <mods/platform.h>
#include <kernel/arch/processor.h>
#include <kernel/locking/lockrank.h>


VALIDATE_IS_X86()

namespace Kernel 
{
    class Spinlock 
    {
        MOD_MAKE_NONCOPYABLE(Spinlock);
        MOD_MAKE_NONMOVABLE(Spinlock);

    public:
        /**
         * @brief Construct a new Spinlock object
         * 
         * @param rank 
         */
        Spinlock(LockRank rank = LockRank::None)
            : m_rank(rank)
        {
        }

        u32 lock();
        void unlock(u32 prev_flags);

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked() const
        {
            return m_lock.load(Mods::memory_order_relaxed) != 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void initialize()
        {
            m_lock.store(0, Mods::memory_order_relaxed);
        }

    private:
        Atomic<u8> m_lock { 0 };
        const LockRank m_rank;
    }; // class SpinLock

    class RecursiveSpinlock 
    {
        MOD_MAKE_NONCOPYABLE(RecursiveSpinlock);
        MOD_MAKE_NONMOVABLE(RecursiveSpinlock);

    public:
        /**
         * @brief Construct a new Recursive Spinlock object
         * 
         * @param rank 
         */
        RecursiveSpinlock(LockRank rank = LockRank::None)
            : m_rank(rank)
        {
        }

        u32 lock();

        /**
         * @param prev_flags 
         */
        void unlock(u32 prev_flags);

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked() const
        {
            return m_lock.load(Mods::memory_order_relaxed) != 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked_by_current_processor() const
        {
            return m_lock.load(Mods::memory_order_relaxed) == FlatPtr(&Processor::current());
        }

        ALWAYS_INLINE void initialize()
        {
            m_lock.store(0, Mods::memory_order_relaxed);
        }

    private:
        Atomic<FlatPtr> m_lock { 0 };
        u32 m_recursions { 0 };
        const LockRank m_rank;
    }; // class RecursiveSpinlock
} // namespace Kernel