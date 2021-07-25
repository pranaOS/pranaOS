/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>

namespace Base {

template<typename T>
concept PointerTypeName = IsPointer<T>;

template<PointerTypeName T>
class Userspace {
public:
    Userspace() = default;

    operator bool() const { return m_ptr; }
    operator FlatPtr() const { return (FlatPtr)m_ptr; }

#ifdef KERNEL
    Userspace(FlatPtr ptr)
        : m_ptr(ptr)
    {
    }

    FlatPtr ptr() const { return m_ptr; }
    T unsafe_userspace_ptr() const { return (T)m_ptr; }
#else
    Userspace(T ptr)
        : m_ptr(ptr)
    {
    }

    T ptr() const { return m_ptr; }
#endif

private:
#ifdef KERNEL
    FlatPtr m_ptr { 0 };
#else
    T m_ptr { nullptr };
#endif

};

}