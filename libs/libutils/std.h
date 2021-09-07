/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/traits.h>
#include <stddef.h>

#if __CONFIG_IS_HOSTED__ == 0

inline void *operator new(size_t, void *ptr)
{
    return ptr;
}

inline void *operator new[](size_t, void *ptr)
{
    return ptr;
}

#else
#   include <new>
#endif

namespace std
{

// FIXME: add more templates

using nullptr_t = decltype(nullptr);

template <typename T>
void swap(T &left, T &right)
{
    T tmp = move(left);
    left = move(right);
    right = move(tmp);
}

template <typename T>
struct initialize_list
{
private:
    T *_data;

size_t _size;

constexpr initialize_list(const T *data, size_t size)
    : _data(data), _size(size)
{
}

public:
    size_t size() const { return _size;}

    constexpr initialize_list()
        : _data(0), _size(0)
    {
    }

    const T &operator[](size_t index) const
    {
        return _data[index];
    }

    T &operator[](size_t index)
    {
        return _data[index];
    }

    constexpr const T *begin() const { return _data; }

    constexpr const T *end() const { return _data + _size; }
};


}