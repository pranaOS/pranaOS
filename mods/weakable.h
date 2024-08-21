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

#ifndef WEAKABLE_DEBUG
#    define WEAKABLE_DEBUG
#endif

namespace Mods 
{

    template<typename T>
    class Weakable;
    template<typename T>
    class WeakPtr;

    class WeakLink : public RefCounted<WeakLink> 
    {
        template<typename T>
        friend class Weakable;

        template<typename T>
        friend class WeakPtr;

    public:
        /**
         * @tparam T 
         * @tparam PtrTraits 
         * @return RefPtr<T, PtrTraits> 
         */
        template<typename T, typename PtrTraits = RefPtrTraits<T>>
        RefPtr<T, PtrTraits> strong_ref() const
        {
            RefPtr<T, PtrTraits> ref;

            {
    #ifdef KERNEL
                Kernel::ScopedCritical critical;
    #endif
                FlatPtr bits = RefPtrTraits<void>::lock(m_bits);
                T* ptr = static_cast<T*>(RefPtrTraits<void>::as_ptr(bits));
                if (ptr)
                    ref = *ptr;
                RefPtrTraits<void>::unlock(m_bits, bits);
            }

            return ref;
        }

        /**
         * @tparam T 
         * @return T* 
         */
        template<typename T>
        T* unsafe_ptr() const
        {
            return static_cast<T*>(RefPtrTraits<void>::as_ptr(m_bits.load(Mods::MemoryOrder::memory_order_acquire)));
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const
        {
            return RefPtrTraits<void>::is_null(m_bits.load(Mods::MemoryOrder::memory_order_relaxed));
        }

        /**
         * @brief revoke
         * 
         */
        void revoke()
        {
            RefPtrTraits<void>::exchange(m_bits, RefPtrTraits<void>::default_null_value);
        }

    private:
        /**
         * @tparam T 
         * @param weakable 
         */
        template<typename T>
        explicit WeakLink(T& weakable)
            : m_bits(RefPtrTraits<void>::as_bits(&weakable))
        {}

        mutable Atomic<FlatPtr> m_bits;
    };

    template<typename T>
    class Weakable 
    {
    private:
        class Link;

    public:
        /**
         * @tparam U 
         * @return WeakPtr<U> 
         */
        template<typename U = T>
        WeakPtr<U> make_weak_ptr() const;

    protected:
        Weakable() { }

        /**
         * @brief Destroy the Weakable object
         * 
         */
        ~Weakable()
        {
    #ifdef WEAKABLE_DEBUG
            m_being_destroyed = true;
    #endif
            revoke_weak_ptrs();
        }

        /**
         * @returns *void
         * 
         */
        void revoke_weak_ptrs()
        {
            if (m_link)
                m_link->revoke();
        }

    private:
        mutable RefPtr<WeakLink> m_link;
    #ifdef WEAKABLE_DEBUG
        bool m_being_destroyed { false };
    #endif
    };

}

using Mods::Weakable;
