/*
 * Copyright (c) 2021, Alex5xt, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <libutils/Std.h>

namespace Utils
{

template <typename T>
struct OwnPtr
{
private:
    T *_ptr = nullptr;

public:
    OwnPtr() {}
    OwnPtr(nullptr_t) {}

    OwnPtr(T *ptr) : _ptr(ptr) {}

    OwnPtr(const OwnPtr &other) : _ptr(const_cast<OwnPtr &>(other).give_ref()) {}

    OwnPtr(OwnPtr &&other) : _ptr(other.give_ref()) {}

    template <typename U>
    OwnPtr(const OwnPtr<U> &other) : _ptr(static_cast<U *>(const_cast<OwnPtr<U>>(other).give_ref())) {}

    template <typename U>
    OwnPtr(OwnPtr<U> &&other) : _ptr(static_cast<U *>(other.give_ref())) {}

    ~OwnPtr()
    {
        if (_ptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    OwnPtr &operator=(OwnPtr &other)
    {
        if (naked() != other.naked())
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    template <typename U>
    OwnPtr &operator=(OwnPtr<U> &other)
    {
        if (naked() != other.naked())
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    OwnPtr &operator=(OwnPtr &&other)
    {
        if (this != &other)
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }
    
};
