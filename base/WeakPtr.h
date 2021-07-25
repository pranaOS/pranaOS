/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <AK/Weakable.h>

namespace AK {

template<typename T>
class WeakPtr {
    template<typename U>
    friend class Weakable;

public:
    WeakPtr() = default;

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(const WeakPtr<U>& other)
        : m_link(other.m_link)
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(WeakPtr<U>&& other)
        : m_link(other.take_link())
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(WeakPtr<U>&& other)
    {
        m_link = other.take_link();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(const WeakPtr<U>& other)
    {
        if ((const void*)this != (const void*)&other)
            m_link = other.m_link;
        return *this;
    }

    WeakPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(const U& object)
        : m_link(object.template make_weak_ptr<U>().take_link())
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(const U* object)
    {
        if (object)
            m_link = object->template make_weak_ptr<U>().take_link();
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(const RefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template make_weak_ptr<U>().take_link();
        });
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr(const NonnullRefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template make_weak_ptr<U>().take_link();
        });
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(const U& object)
    {
        m_link = object.template make_weak_ptr<U>().take_link();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(const U* object)
    {
        if (object)
            m_link = object->template make_weak_ptr<U>().take_link();
        else
            m_link = nullptr;
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(const RefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template make_weak_ptr<U>().take_link();
            else
                m_link = nullptr;
        });
        return *this;
    }


}