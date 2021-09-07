/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/func.h>

namespace Utils
{

enum struct Iteration
{
    CONTINUE,
    STOP,
};

template <typename T>
using IterFunc = Func<Iteration(T)>;

template <typename T>
struct ContiguousIterator
{
private:
    T *_ptr;

public:
    ContiguousIterator(T *ptr) : _ptr(ptr) {}

    ContiguousIterator<T> operator++()
    {
        ++_ptr;
        return *this;
    }

    bool operator!=(const ContiguousIterator<T> &other) const
    {
        return _ptr != other._ptr;
    }

    const T &operator*() const
    {
        return *_ptr;
    }
};

}