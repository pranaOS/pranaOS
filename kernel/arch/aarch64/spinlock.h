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

#include <mods/noncopyable.h>
#include <mods/types.h>
#include <kernel/locking/lockrank.h>

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
        {
            (void)rank;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 lock()
        {
            return 0;
        }

        ALWAYS_INLINE void unlock(u32)
        {
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked() const
        {
            return false;
        }

        ALWAYS_INLINE void initialize()
        {
        }
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
        {
            (void)rank;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 lock()
        {
            return 0;
        }

        ALWAYS_INLINE void unlock(u32)
        {
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked() const
        {
            return false;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool is_locked_by_current_processor() const
        {
            return false;
        }

        ALWAYS_INLINE void initialize()
        {
        }
    }; // class RecursiveSpinLock
} // namespace Kernel