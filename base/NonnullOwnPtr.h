/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Assertions.h>
#include <base/Format.h>
#include <base/RefCounted.h>
#include <base/StdLibExtras.h>
#include <base/Traits.h>
#include <base/Types.h>

namespace Base {

template<typename T, typename PtrTraits>
class RefPtr;
template<typename T>
class NonnullRefPtr;
template<typename T>
class WeakPtr;

template<typename T>
class NonnullRefPtr {
public:
    using ElementType = T;

    enum AdoptTag { Adopt };

    NonnullOwnPtr(AdoptTag, T& ptr)
    : m_ptr(&ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } || !requires { requires !typename T::AllowOwnPtr()(); declval<T>().ref(); declval<T>().unref(); },
                "Use NonnullRefPtr<> for RefCounted types");
        }
    NonnullOwnPtr(NonnullOwnPtr&& other)
    : m_ptr(other.leak_ptr())
    {
        VERIFY(m_ptr);
    }
    template<typename U>
    NonnullOwnPtr(NonnullOwnPtr<U>&& other)
    : m_ptr(other.leak_ptr())
    {
        VERIFY(m_ptr);
    }
    ~NonnullOwnPtr()
    {
        clear();
#ifdef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_ptr = (T*)(0xe3e3e3e3e3e3e3e3);
        else
            m_ptr = (T*)(0xe3e3e3e3);
#endif
    }

    NonnullOwnPtr(const NonnullOwnPtr&) = delete;
    template<typename U>
    NonnullOwnPtr(const NonnullOwnPtr<U>&) = delete;
    NonnullOwnPtr& operator=(const NonnullOwnPtr&) = delete;
    template<typename U>
    NonnullOwnPtr& operator=(const NonnullOwnPtr<U>&) = delete;

    template<typename U, typename PtrTraits = RefPtrTraits<U>>
    NonnullOwnPtr(const RefPtr<U, PtrTraits>&) = delete;
    template<typename U>
    NonnullOwnPtr(const NonnullRefPtr<U>&) = delete;
    template<typename U>
    NonnullOwnPtr(const WeakPtr<U>&) = delete;
    template<typename U, typename PtrTraits = RefPtrTraits<U>>
    NonnullOwnPtr& operator=(const RefPtr<U, PtrTraits>&) = delete;
    template<typename U>
    NonnullOwnPtr& operator=(const NonnullRefPtr<U>&) = delete;
    template<typename U>
    NonnullOwnPtr& operator=(const WeakPtr<U>&) = delete;

};

}