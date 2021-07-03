/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/Func.h>

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

}

}