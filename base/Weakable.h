/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "Assertions.h"
#include "Atomic.h"
#include "RefCounted.h"
#include "RefPtr.h"
#include "StdLibExtras.h"
#ifdef KERNEL
#    include <kernel/arch/x86/Processor.h>
#    include <kernel/arch/x86/ScopedCritical.h>
#endif

namespace Base {

template<typename T>
class Weakable;
template<typename T>
class WeakPtr;

class WeakLink : public RefCounted<WeakLink> {
    template<typename T>
    friend class Weakable;
    template<typename T>
    friend class WeakPtr;

public:
    template<typename T, typename PtrTraits = RefPtrTraits<T>, typename EnableIf<IsBaseOf<RefCountedBase, T>>::Type* = nullptr>
    RefPtr<T, PtrTraits> strong_ref() const
    {
        RefPtr<T, PtrTraits> ref;

        {
#ifdef KERNEL
            
            kernel::ScopedCritical critical;
#endif
            if (!(m_consumers.fetch_add(1u << 1, Base::MemoryOrder::memory_order_acquire) & 1u)) {
                T* ptr = (T*)m_ptr.load(Base::MemoryOrder::memory_order_acquire);
                if (ptr && ptr->try_ref())
                    ref = adopt_ref(*ptr);
            }
            m_consumers.fetch_sub(1u << 1, Base::MemoryOrder::memory_order_release);
        }

        return ref;
    }

    template<typename T>
    T* unsafe_ptr() const
    {
        if (m_consumers.load(Base::MemoryOrder::memory_order_relaxed) & 1u)
            return nullptr;

        return (T*)m_ptr.load(Base::MemoryOrder::memory_order_acquire);
    }

    bool is_null() const
    {
        return !unsafe_ptr<void>();
    }

    void revoke()
    {
        auto current_consumers = m_consumers.fetch_or(1u, Base::MemoryOrder::memory_order_relaxed);
        VERIFY(!(current_consumers & 1u));

        while (current_consumers > 0) {
#ifdef KERNEL
            kernel::Processor::wait_check();
#else
            
#endif
            current_consumers = m_consumers.load(Base::MemoryOrder::memory_order_acquire) & ~1u;
        }

        m_ptr.store(nullptr, Base::MemoryOrder::memory_order_release);
    }

private:
    template<typename T>
    explicit WeakLink(T& weakable)
        : m_ptr(&weakable)
    {
    }
    mutable Atomic<void*> m_ptr;
    mutable Atomic<unsigned> m_consumers; 
};

template<typename T>
class Weakable {
private:
    class Link;

public:
    template<typename U = T>
    WeakPtr<U> make_weak_ptr() const;

protected:
    Weakable() = default;

    ~Weakable()
    {
        m_being_destroyed.store(true, Base::MemoryOrder::memory_order_release);
        revoke_weak_ptrs();
    }

    void revoke_weak_ptrs()
    {
        if (auto link = move(m_link))
            link->revoke();
    }

private:
    mutable RefPtr<WeakLink> m_link;
    Atomic<bool> m_being_destroyed { false };
};

}

using Base::Weakable;