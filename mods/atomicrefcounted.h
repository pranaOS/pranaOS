/**
 * @file atomicrefcounted.h
 * @author Krisna Pranav
 * @brief atomic ref counted
 * @version 6.0
 * @date 2025-03-13
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/checked.h>
#include <mods/noncopyable.h>
#include <mods/platform.h>

namespace Mods 
{

    class AtomicRefCountedBase 
    {
        MODS_MAKE_NONCOPYABLE(AtomicRefCountedBase);
        MODS_MAKE_NONMOVABLE(AtomicRefCountedBase);

    public:
        using RefCountType = unsigned int;
        using AllowOwnPtr = FalseType;

        void ref() const
        {
            auto old_ref_count = m_ref_count.fetch_add(1, Mods::MemoryOrder::memory_order_relaxed);
            VERIFY(old_ref_count > 0);
            VERIFY(!Checked<RefCountType>::addition_would_overflow(old_ref_count, 1));
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool try_ref() const
        {
            RefCountType expected = m_ref_count.load(Mods::MemoryOrder::memory_order_relaxed);
            for (;;) {
                if (expected == 0)
                    return false;
                VERIFY(!Checked<RefCountType>::addition_would_overflow(expected, 1));
                if (m_ref_count.compare_exchange_strong(expected, expected + 1, Mods::MemoryOrder::memory_order_acquire))
                    return true;
            }
        }

        /**
         * @return RefCountType 
         */
        [[nodiscard]] RefCountType ref_count() const
        {
            return m_ref_count.load(Mods::MemoryOrder::memory_order_relaxed);
        }

    protected:
        /**
         * @brief Construct a new AtomicRefCountedBase object
         * 
         */
        AtomicRefCountedBase() = default;

        /**
         * @brief Destroy the AtomicRefCountedBase object
         * 
         */
        ~AtomicRefCountedBase()
        {
            VERIFY(m_ref_count.load(Mods::MemoryOrder::memory_order_relaxed) == 0);
        }

        /**
         * @return RefCountType 
         */
        RefCountType deref_base() const
        {
            auto old_ref_count = m_ref_count.fetch_sub(1, Mods::MemoryOrder::memory_order_acq_rel);
            VERIFY(old_ref_count > 0);
            return old_ref_count - 1;
        }

        mutable Atomic<RefCountType> m_ref_count { 1 };
    }; // class AtomicRefCountedBase

    /**
     * @tparam T 
     */
    template<typename T>
    class AtomicRefCounted : public AtomicRefCountedBase 
    {
    public:
        bool unref() const
        {
            auto* that = const_cast<T*>(static_cast<T const*>(this));
            auto new_ref_count = deref_base();
            if (new_ref_count == 0) {
                if constexpr (requires { that->will_be_destroyed(); })
                    that->will_be_destroyed();
                delete that;
                return true;
            }
            return false;
        }
    }; // class AtomicRefCounted : public AtomicRefCountedBase 

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::AtomicRefCounted;
using Mods::AtomicRefCountedBase;
#endif
