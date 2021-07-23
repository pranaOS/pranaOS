/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Format.h>
#include <base/NonnullRefPtr.h>
#include <base/StdLibExtras.h>
#include <base/Traits.h>
#include <base/Types.h>
#ifdef KERNEL
#    include <kernel/arch/x86/Processor.h>
#    include <kernel/arch/x86/ScopedCritical.h>
#endif


namespace Base {

template<typename T>
class OwnPtr;

template<typename T>
struct RefPtrTraits {
    ALWAYS_INLINE static T* as_ptr(FlatPtr bits)
    {
        return (T*)(bits & ~(FlatPtr)1);
    }

    ALWAYS_INLINE static FlatPtr as_bits(T* ptr)
    {
        VERIFY(!((FlatPtr)ptr & 1));
        return (FlatPtr)ptr;
    }

    template<typename U, typename PtrTraits>
    ALWAYS_INLINE static FlatPtr convert_from(FlatPtr bits)
    {
        if (PtrTraits::is_null(bits))
            return default_null_value;
        return as_bits(PtrTraits::as_ptr(bits));
    }

    ALWAYS_INLINE static bool is_null(FlatPtr bits)
    {
        return !(bits & ~(FlatPtr)1);
    }

    ALWAYS_INLINE static FlatPtr exchange(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        FlatPtr expected = atomic_var.load(Base::MemoryOrder::memory_order_relaxed);
        for (;;) {
            expected &= ~(FlatPtr)1; 
            if (atomic_var.compare_exchange_strong(expected, new_value, Base::MemoryOrder::memory_order_acq_rel))
                break;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        return expected;
    }

    ALWAYS_INLINE static bool exchange_if_null(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        for (;;) {
            FlatPtr expected = default_null_value; 
            if (atomic_var.compare_exchange_strong(expected, new_value, Base::MemoryOrder::memory_order_acq_rel))
                break;
            if (!is_null(expected))
                return false;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        return true;
    }

    ALWAYS_INLINE static FlatPtr lock(Atomic<FlatPtr>& atomic_var)
    {

        FlatPtr bits;
        for (;;) {
            bits = atomic_var.fetch_or(1, Base::MemoryOrder::memory_order_acq_rel);
            if (!(bits & 1))
                break;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        VERIFY(!(bits & 1));
        return bits;
    }

    ALWAYS_INLINE static void unlock(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        atomic_var.store(new_value, Base::MemoryOrder::memory_order_release);
    }

    static constexpr FlatPtr default_null_value = 0;

    using NullType = std::nullptr_t;
};

}