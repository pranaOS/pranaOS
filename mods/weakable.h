/**
 * @file weakable.h
 * @author Krisna Pranav
 * @brief weakable
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "assertions.h"
#include "atomic.h"
#include "refcounted.h"
#include "refptr.h"
#include "stdlibextra.h"
#ifdef KERNEL
#include <kernel/arch/processor.h>
#include <kernel/arch/scopedcritical.h>
#else
#include <sched.h>
#endif

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class Weakable;

    /**
     * @tparam T 
     */
    template <typename T>
    class WeakPtr;

    class WeakLink : public RefCounted<WeakLink>
    {
        /**
         * @tparam T 
         */
        template <typename T>
        friend class Weakable;
        template <typename T>
        friend class WeakPtr;

    public:
        /**
         * @tparam T 
         * @tparam PtrTraits 
         */
        template <typename T, typename PtrTraits = RefPtrTraits<T>>
        RefPtr<T, PtrTraits> strong_ref() const
            requires(IsBaseOf<RefCountedBase, T>)
        {
            RefPtr<T, PtrTraits> ref;

            {
    #ifdef KERNEL
                Kernel::ScopedCritical critical;
    #endif
                if(!(m_consumers.fetch_add(1u << 1, Mods::MemoryOrder::memory_order_acquire) & 1u))
                {
                    T* ptr = (T*)m_ptr.load(Mods::MemoryOrder::memory_order_acquire);
                    if(ptr && ptr->try_ref())
                        ref = adopt_ref(*ptr);
                }
                m_consumers.fetch_sub(1u << 1, Mods::MemoryOrder::memory_order_release);
            }

            return ref;
        }

        /**
         * @tparam T 
         * @return T* 
         */
        template <typename T>
        T* unsafe_ptr() const
        {
            if(m_consumers.load(Mods::MemoryOrder::memory_order_relaxed) & 1u)
                return nullptr;
            
            return (T*)m_ptr.load(Mods::MemoryOrder::memory_order_acquire);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const
        {
            return unsafe_ptr<void>() == nullptr;
        }

        void revoke()
        {
            auto current_consumers = m_consumers.fetch_or(1u, Mods::MemoryOrder::memory_order_relaxed);
            VERIFY(!(current_consumers & 1u));
            
            while(current_consumers > 0)
            {
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #else
                sched_yield();
    #endif
                current_consumers = m_consumers.load(Mods::MemoryOrder::memory_order_acquire) & ~1u;
            }
            
            m_ptr.store(nullptr, Mods::MemoryOrder::memory_order_release);
        }

    private:
        /**
         * @brief Construct a new Weak Link object
         * 
         * @tparam T 
         * @param weakable 
         */
        template <typename T>
        explicit WeakLink(T& weakable)
            : m_ptr(&weakable)
        {
        }

        mutable Atomic<void*> m_ptr;
        mutable Atomic<unsigned> m_consumers; 
    };

    /**
     * @tparam T 
     */
    template <typename T>
    class Weakable
    {
    private:
        class Link;

    public:
    #ifndef KERNEL
        template <typename U = T>
        WeakPtr<U> make_weak_ptr() const
        {
            return MUST(try_make_weak_ptr<U>());
        }
    #endif
        template <typename U = T>
        ErrorOr<WeakPtr<U>> try_make_weak_ptr() const;

    protected:
        Weakable() = default;

        ~Weakable()
        {
    #ifdef KERNEL
            m_being_destroyed.store(true, Mods::MemoryOrder::memory_order_release);
    #endif
            revoke_weak_ptrs();
        }

        void revoke_weak_ptrs()
        {
            if(auto link = move(m_link))
                link->revoke();
        }

    private:
        mutable RefPtr<WeakLink> m_link;
    #ifdef KERNEL
        Atomic<bool> m_being_destroyed{false};
    #endif
    }; // class Weakable
} // namespace Mods

using Mods::Weakable;
