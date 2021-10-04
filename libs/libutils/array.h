/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libutils/iterator.h>
#include <libutils/span.h>

namespace Utils {

template <typename T, size_t Size>
struct Array {
    [[nodiscard]] constexpr T const* data() const { return __data; }
    [[nodiscard]] constexpr T* data() { return __data; }

    [[nodiscard]] constexpr size_t size() const { return Size; }

    [[nodiscard]] constexpr Span<T const> span() const { return { __data, Size}; }
    [[nodiscard]] constexpr Span<T> span() { return { __data, Size }; }

    [[nodiscard]] constexpr T const& at(size_t index) const
    {
        VERIFY(index < size());
        return __data[index];
    }

    [[nodiscard]] constexpr T& at(size_t index)
    {
        VERIFY(index < size());
        return __data[index];
    }

    constexpr size_t fill(T const& value)
    {
        for (size_t idx = 0; idx < Size; ++idx)
            __data[idx] = value;
        
        return Size;
    }

};

template <typename T, typename... Types>
Array(T, Types...) -> Array<T, sizeof...(Types) + 1>;

namespace Detail {
template <typename T, size_t... Is>
constexpr auto integer_sequence_generate_array([[maybe_unused]] T const offset, IntegerSequence<T, Is...>) -> Array<T, sizeof...(Is)>
{
    return { { (offset + Is)... } };
}
}

template<typename T, T N>
constexpr static auto iota_array(T const offset = {})
{
    static_assert(N >= T {}, "Negative sizes not allowed in iota_array()");
    return Detail::integer_sequence_generate_array<T>(offset, MakeIntegerSequence<T, N>());
}

}

using Utils::Array;
using Utils::iota_array;