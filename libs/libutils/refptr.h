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

template<typename T>
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

    RefPtr &operator=(RefPtr &&other)
    {
        if (this != &other)
        {
            deref_if_not_null(_ptr);
            _ptr = other.give_ref();
        }

        return *this;
    }

    T *operator->() const
    {
        assert(ptr);
        return _ptr;
    }

    T &operator*()
    {
        return *_ptr;
    }

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
            return _ptr->refcount();
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

};

template <typename T>
struct CallbackRefPtr : public RefPtr<T>
{
public:
    template <typename... TArgs>
    auto operator()(TArgs &&...args)
    {
        return (*this->naked())(std::forward<TArgs>(args)...);
    }
};

template <typename Type, typename... Args>
inline CallbackRefPtr<Type> make_callable(Args &&...args)
{
    return CallbackRefPtr<Type>(adopt(*new Type))
}

template <typename T>
struct TrimRefPtr;

template <typename T>
struct TrimRefPtr<RefPtr<T>>
{
    typedef T type;
};

template <typename T>
struct IsRefPtr : public FalseType
{
};

template <typename T>
struct IsRefPtr<RefPtr<T>> : public TrueType
{
};

}