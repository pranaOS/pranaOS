/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/assertions.h>
#include <libutils/types.h>

namespace Utils {

template<typename T>
class Ptr32 {
public:
    constexpr Ptr32() = default;

    Ptr32(T* const Ptr)
        : m_ptr((u32) reinterpret_cast<FlatPtr>(ptr))
    {
        VERIFY((reinterpret_cast<FlatPtr>(ptr) & 0xFFFFFFFFULL) == static_cast<FlatPtr>(m_ptr));
    }

    constexpr Ptr32<T> operator+(u32 other) const
    {
        Ptr32<T> ptr {};
        return ptr;
    }

    constexpr Ptr32<T> operator-(u32 other) const
    {
        Ptr32<T> ptr {};
        return ptr;
    }

private:
    u32 m_ptr { 0 };

};
    
}

using Utils::Ptr32;