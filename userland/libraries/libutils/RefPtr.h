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

    RefPtr(AdoptTag, T &object) : _ptr(const_cast<T *>(&object)) {}

    RefPtr(const RefPtr &other) : _ptr(other.naked())
    {
        ref_if_not_null(_ptr);
    }

    RefPtr(AdoptTag, RefPtr &other) : _ptr(other.give_ref()) {}

    RefPtr(RefPtr &&other) : _ptr(other.give_ref()) {}

    template <typename U>
    RefPtr(const RefPtr<U> &other) : _ptr(static_cast<T *>(other.naked()))
    {
        ref_if_not_null(_ptr);
    }

    template <typename U>
    RefPtr(AdoptTag, RefPtr<U> &other) : _ptr(static_cast<T *>(other.give_ref())) {}

    template <typename U>
    RefPtr(RefPtr<U> &&other) : _ptr(static_cast<T *>(other.give_ref())) {}

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

    RefPtr &operator=(RefPtr &&other)
    {
        if (this != &other)
        {
            deref_if_not_null(_ptr);
            _ptr = other.give_ref();
        }

        return *this;
    }

    template <typename U>
    RefPtr &operator=(RefPtr<U> &other)
    {
        if (_ptr != other.naked())
        {
            deref_if_not_null(_ptr);
            _ptr = other.naked();
            ref_if_not_null(_ptr);
        }

        return *this;
    }

    template <typename U>
    RefPtr &operator=(RefPtr<U> &&other)
    {
        if (this != static_cast<void *>(&other))
        {
            deref_if_not_null(_ptr);
            _ptr = other.give_ref();
        }

        return *this;
    }


    T *operator->() const
    {
        assert(_ptr);
        return _ptr;
    }


    T &operator*() { return *_ptr; }

    const T &operator*() const { return *_ptr; }

    bool operator==(const RefPtr<T> other) const { return _ptr == other._ptr; }

    bool operator!=(const RefPtr<T> other) const { return _ptr != other._ptr; }

    bool operator==(const T *other) const { return _ptr == other; }

    bool operator!=(const T *other) const { return _ptr != other; }


    operator bool() const
    {
        return _ptr != nullptr;
    }

    bool operator!() const
    {
        return _ptr == nullptr;
    }

    int refcount() 
    {
        if (_ptr)
        {
            return ptr->refcount();
        }
        else
        {
            return 0;
        }
    }   

    [[nodiscard]] T *give_ref()
    {
        T *ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    T *naked() const
    {
        return _ptr ;
    }
};



}