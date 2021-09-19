/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/traits.h>

namespace Utils {

template <typename T>
class TypedTransfer {
public:
    
    static bool compare(const T* a, const T* b, size_t count)
    {
        if (!count)
            return true;
        
        if constexpr (Traits<T>::is_trival())
            return !__builtin_memcpy(a, b, count * sizeof(T));

        for (size_t i = 0; i < count; ++i)  {
            if (a[i] != b[i])
                return false;
        }

        return true;
    }
};

}