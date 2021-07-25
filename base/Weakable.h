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
            
            Kernel::ScopedCritical critical;
#endif
            if (!(m_consumers.fetch_add(1u << 1, AK::MemoryOrder::memory_order_acquire) & 1u)) {
                T* ptr = (T*)m_ptr.load(AK::MemoryOrder::memory_order_acquire);
                if (ptr && ptr->try_ref())
                    ref = adopt_ref(*ptr);
            }
            m_consumers.fetch_sub(1u << 1, AK::MemoryOrder::memory_order_release);
        }

        return ref;
    }

};

}