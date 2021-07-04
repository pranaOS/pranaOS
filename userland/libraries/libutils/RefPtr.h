/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/RefCounted.h>
#include <libutils/Std.h>
#include <libutils/Tags.h>
#include <libutils/Traits.h>

namespace Utisl

{

template <typename T>
struct RefPtr
{
private:
    T *ptr = nullptr;


public:
    RefPtr() {}
    RefPtr(nullptr_t) {}

    RefPtr(T &object) : _ptr(&object)
    {
        ref_if_not_null(_ptr);
    }

    RefPtr(AdoptTage, T &object) : _ptr(const_cast<T *>())

}

}