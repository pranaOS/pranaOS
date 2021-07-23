/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/NonnullOwnPtr.h>
#include <base/RefCounted.h>

namespace Base {

template<typename T>
class OwnPtr {
public
    OwnPtr() = default;

    OwnPtr(decltype(nullptr))
        : m_ptr(nullptr)
    {
    }

    OwnPtr(OwnPtr&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    template<typename U>
    OwnPtr(NonnullOwnPtr<U>&& other)
        : m_ptr(other.leak_ptr())
    {
    }
    template<typename U>
    OwnPtr(OwnPtr<U>&& other)
        : m_ptr(other.leak_ptr())
    {
    }
    ~OwnPtr()
    {
        clear();
#ifdef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_ptr = (T*)(0xe1e1e1e1e1e1e1e1);
        else
            m_ptr = (T*)(0xe1e1e1e1);
#endif
    }

    OwnPtr(const OwnPtr&) = delete;
    template<typename U>
    OwnPtr(const OwnPtr<U>&) = delete;
    OwnPtr& operator=(const OwnPtr&) = delete;
    template<typename U>
    OwnPtr& operator=(const OwnPtr<U>&) = delete;

    template<typename U>
    OwnPtr(const NonnullOwnPtr<U>&) = delete;
    template<typename U>
    OwnPtr& operator=(const NonnullOwnPtr<U>&) = delete;
    template<typename U>
    OwnPtr(const RefPtr<U>&) = delete;
    template<typename U>
    OwnPtr(const NonnullRefPtr<U>&) = delete;
    template<typename U>
    OwnPtr(const WeakPtr<U>&) = delete;
    template<typename U>
    OwnPtr& operator=(const RefPtr<U>&) = delete;
    template<typename U>
    OwnPtr& operator=(const NonnullRefPtr<U>&) = delete;
    template<typename U>
    OwnPtr& operator=(const WeakPtr<U>&) = delete;

    OwnPtr& operator=(OwnPtr&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<typename U>
    OwnPtr& operator=(OwnPtr<U>&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<typename U>
    OwnPtr& operator=(NonnullOwnPtr<U>&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        VERIFY(m_ptr);
        return *this;
    }

    OwnPtr& operator=(T* ptr) = delete;

    OwnPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

};

}
