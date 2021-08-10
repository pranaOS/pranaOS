/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Weakable.h>

namespace Base {

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

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeakPtr& operator=(const NonnullRefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template make_weak_ptr<U>().take_link();
            else
                m_link = nullptr;
        });
        return *this;
    }

    [[nodiscard]] RefPtr<T> strong_ref() const
    {

        RefPtr<T> ref;
        
        m_link.do_while_locked([&](WeakLink* link) {
            if (link)
                ref = link->template strong_ref<T>();
        });
        return ref;
    }

#ifndef KERNEL
    
    T* ptr() const { return unsafe_ptr(); }
    T* operator->() { return unsafe_ptr(); }
    const T* operator->() const { return unsafe_ptr(); }
    operator const T*() const { return unsafe_ptr(); }
    operator T*() { return unsafe_ptr(); }
#endif

    [[nodiscard]] T* unsafe_ptr() const
    {
        T* ptr = nullptr;
        m_link.do_while_locked([&](WeakLink* link) {
            if (link)
                ptr = link->unsafe_ptr<T>();
        });
        return ptr;
    }

    operator bool() const { return m_link ? !m_link->is_null() : false; }

    [[nodiscard]] bool is_null() const { return !m_link || m_link->is_null(); }
    void clear() { m_link = nullptr; }

    [[nodiscard]] RefPtr<WeakLink> take_link() { return move(m_link); }

private:
    WeakPtr(const RefPtr<WeakLink>& link)
        : m_link(link)
    {
    }

    RefPtr<WeakLink> m_link;
};

template<typename T>
template<typename U>
inline WeakPtr<U> Weakable<T>::make_weak_ptr() const
{
    if constexpr (IsBaseOf<RefCountedBase, T>) {

        if (!static_cast<const T*>(this)->try_ref())
            return {};
    } else {

        if (m_being_destroyed.load(Base::MemoryOrder::memory_order_acquire))
            return {};
    }
    if (!m_link) {

        m_link.assign_if_null(adopt_ref(*new WeakLink(const_cast<T&>(static_cast<const T&>(*this)))));
    }

    WeakPtr<U> weak_ptr(m_link);

    if constexpr (IsBaseOf<RefCountedBase, T>) {

        if (static_cast<const T*>(this)->unref()) {

            VERIFY(!weak_ptr.strong_ref());
            return {};
        }
    }
    return weak_ptr;
}

template<typename T>
struct Formatter<WeakPtr<T>> : Formatter<const T*> {
    void format(FormatBuilder& builder, const WeakPtr<T>& value)
    {
#ifdef KERNEL
        auto ref = value.strong_ref();
        Formatter<const T*>::format(builder, ref.ptr());
#else
        Formatter<const T*>::format(builder, value.ptr());
#endif
    }
};

template<typename T>
WeakPtr<T> try_make_weak_ptr(const T* ptr)
{
    if (ptr) {
        return ptr->template make_weak_ptr<T>();
    }
    return {};
}

}

using Base::WeakPtr;
