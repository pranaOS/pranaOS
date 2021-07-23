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
#include <base/Traits.h>
#include <base/Types.h>
#ifdef KERNEL
#    include <kernel/arch/x86/Processor.h>
#    include <kernel/arch/x86/ScopedCritical.h>
#endif

namespace Base {

template<typename T>
class OwnPtr;
template<typename T, typename PtrTraits>
class RefPtr;

template<typename T>
ALWAYS_INLINE void ref_if_not_null(T* ptr)
{
    if (ptr)
        ptr->ref();
}

template<typename T>
ALWAYS_INLINE void unref_if_not_null(T* ptr)
{
    if (ptr)
        ptr->unref();
}

template<typename T>
class NonnullRefPtr {
    template<typename U, typename P>
    friend class RefPtr;
    template<typename U>
    friend class NonnullRefPtr;
    template<typename U>
    friend class WeakPtr;

public:
    using ElementType = T;

    enum AdoptTag { Adopt };

    ALWAYS_INLINE NonnullRefPtr(const T& object)
        : m_bits((FlatPtr)&object)
    {
        VERIFY(!(m_bits & 1));
        const_cast<T&>(object).ref();
    }
    template<typename U>
    ALWAYS_INLINE NonnullRefPtr(const U& object)
        : m_bits((FlatPtr) static_cast<const T*>(&object))
    {
        VERIFY(!(m_bits & 1));
        const_cast<T&>(static_cast<const T&>(object)).ref();
    }
    ALWAYS_INLINE NonnullRefPtr(AdoptTag, T& object)
        : m_bits((FlatPtr)&object)
    {
        VERIFY(!(m_bits & 1));
    }
    ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr&& other)
        : m_bits((FlatPtr)&other.leak_ref())
    {
        VERIFY(!(m_bits & 1));
    }
    template<typename U>
    ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr<U>&& other)
        : m_bits((FlatPtr)&other.leak_ref())
    {
        VERIFY(!(m_bits & 1));
    }
    ALWAYS_INLINE NonnullRefPtr(const NonnullRefPtr& other)
        : m_bits((FlatPtr)other.add_ref())
    {
        VERIFY(!(m_bits & 1));
    }
    template<typename U>
    ALWAYS_INLINE NonnullRefPtr(const NonnullRefPtr<U>& other)
        : m_bits((FlatPtr)other.add_ref())
    {
        VERIFY(!(m_bits & 1));
    }
    ALWAYS_INLINE ~NonnullRefPtr()
    {
        assign(nullptr);
#ifdef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_bits.store(0xb0b0b0b0b0b0b0b0, AK::MemoryOrder::memory_order_relaxed);
        else
            m_bits.store(0xb0b0b0b0, AK::MemoryOrder::memory_order_relaxed);
#endif
    }

    template<typename U>
    NonnullRefPtr(const OwnPtr<U>&) = delete;
    template<typename U>
    NonnullRefPtr& operator=(const OwnPtr<U>&) = delete;

    template<typename U>
    NonnullRefPtr(const RefPtr<U>&) = delete;
    template<typename U>
    NonnullRefPtr& operator=(const RefPtr<U>&) = delete;
    NonnullRefPtr(const RefPtr<T>&) = delete;
    NonnullRefPtr& operator=(const RefPtr<T>&) = delete;

    NonnullRefPtr& operator=(const NonnullRefPtr& other)
    {
        if (this != &other)
            assign(other.add_ref());
        return *this;
    }

    template<typename U>
    NonnullRefPtr& operator=(const NonnullRefPtr<U>& other)
    {
        assign(other.add_ref());
        return *this;
    }

    ALWAYS_INLINE NonnullRefPtr& operator=(NonnullRefPtr()&& other)
    {
        if (this != &other)
            assign(&other.leak_ref());
        return *this;
    }

    template<typename U>
    NonnullRefPtr& operator=(NonnullRefPtr<U>&& other)
    {
        assign(&other.leak_ref());
        return *this;
    }

};

}