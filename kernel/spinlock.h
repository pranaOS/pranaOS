/**
 * @file spinlock.h
 * @author Krisna Pranav
 * @brief spinlock
 * @version 1.0
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
        MODS_MAKE_NONCOPYABLE(SpinLock);
        MODS_MAKE_NONMOVABLE(SpinLock);

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
            Processor::current().leave_critical(prev_flag);
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
}