/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Base {

template<typename T, typename U>
inline T bit_cast(const U& a)
{
    static_assert(sizeof(T) == sizeof(U));

    T result;
    __builtin_memcpy(&result, &a, sizeof(T));
    return result;
}

}

using Base::bit_cast;