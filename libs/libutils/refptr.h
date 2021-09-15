/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/refcounted.h>
#include <libutils/std.h>
#include <libutils/tags.h>
#include <libutils/traits.h>

namespace Utils
{

template <typename T>
struct RefPtr
{
private:
    T *_ptr = nullptr;

public:
    RefPtr()
    {
    }

    RefPtr(nullptr_t)
    {
    }

    RefPtr(T &object) : _ptr(&object)
    {
        ref_if_not_null(_ptr);
    }

    ~RefPtr()
    {
        deref_if_not_null(_ptr);
        _ptr = nullptr;
    }

    RefPtr &operator=(nullptr_t)
    {
        deref_if_not_null(_ptr);
        _ptr = nullptr;

        return *this;
    }

    RefPtr &operator=(const RefPtr &other)
    {
        if (_ptr != other.naked())
        {
            deref_if_not_null(_ptr);
            _ptr = other.naked();
            ref_if_not_null(_ptr);
        }

        return *this;
    }

    [[nodiscard]] T *give_ref()
    {
        T *ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    T *naked() const
    {
        return _ptr;
    }


};

template <typename T>
struct IsRefPtr : public FalseType
{
};

}