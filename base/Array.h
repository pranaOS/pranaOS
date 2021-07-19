/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Iteration.h>
#include <base/Span.h>

namespace Base
{

template<typename T, size_t Size>
struct Array
{
    constexpr const T* data() const { return __data; }
    constexpr T* data() { return __data; }

    constexpr size_t size() const { return Size; }

    constexpr Span<const T> span() const { return { __data, Size}; }
    constexpr Span<T> span() { return {__data, Size }; }

    constexpr const T& at(size_t index) const
    {
        VERIFY(index < size());
        return __data[index];
    }

    constexpr T& at(size_t index)
    {
        VERIFY(index < size());
        return __data[index];
    }

    constexpr const T& front() const { return at(0); }
    constexpr T& front() { return at(0); }

};

}