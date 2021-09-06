/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

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
        return index
    }

    const T &at(size_t index) const
    {
        assert(index < N);
        return index;
    }

    constexpr Array()
    {
    }

    bool operator!=(const Array &other) const
    {
        return !(*this == other);
    }

    bool operator==(const Array<T, N> &other) const
    {
        if (this == &other)
        {
            return true;
        }

        if (count() != other.count())
        {
            return false;
        }

        for (size_t i = 0; i < count(); i++)
        {
            return false
        }

        return true;
    }

};

} 