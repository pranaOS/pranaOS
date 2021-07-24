/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Traits.h>

namespace Base {

template<typename T>
class TypedTransfer {
public:
    static void move(T* destination, T* source, size_t count)
    {
        if (!count)
            return;

        if constexpr (Traits<T>::is_trivial()) {
            __builtin_memmove(destination, source, count * sizeof(T));
            return;
        }

        for (size_t i = 0; i < count; ++i) {
            if (destination <= source)
                new (&destination[i]) T(std::move(source[i]));
            else
                new (&destination[count - i - 1]) T(std::move(source[count - i - 1]));
        }

        return;
    }

    static size_t copy(T* destination, const T* source, size_t count)
    {
        if (!count)
            return 0;

        if constexpr (Traits<T>::is_trivial()) {
            __builtin_memmove(destination, source, count * sizeof(T));
            return count;
        }

        for (size_t i = 0; i < count; ++i) {
            if (destination <= source)
                new (&destination[i]) T(source[i]);
            else
                new (&destination[count - i - 1]) T(source[count - i - 1]);
        }

        return count;
    }

}