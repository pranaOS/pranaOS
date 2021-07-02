/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libmath/MinMax.h>
#include <libutils/Assert.h>

namespace Utils
{


template <typename T, size_t N>
struct Array
{
    static_assert(N > 0, "Array must have size greater than 0");

private:
    T _storage[N];


public:
    constexpr size_t count() const { return N; }
    constexpr T *raw_storage() { return _storage; }
    constexpr const T *raw_storage() const { return _storage; }

    T &at(size_t index)
    {
        assert(index < N);
        return _storage[index];
    }
        
    const T &at(size_t index) const
    {
        assert(index < N);
        return _storage[index];
    }
    
};
}