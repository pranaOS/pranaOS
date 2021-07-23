/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Checked.h>
#include <base/Noncopyable.h>
#include <base/Platform.h>
#include <base/StdLibExtras.h>

namespace Base {

template<class T>
constexpr auto call_will_be_destroyed_if_present(const T* object) -> decltype(const_cast<T*>(object)->will_be_destroyed(), TrueType {})
{
    const_cast<T*>(object)->will_be_destroyed();
    return {};
}

constexpr auto call_will_be_destroyed_if_present(...) -> FalseType
{
    return {};
}

template<class T>
constexpr auto call_one_ref_left_if_present(const T* object) -> decltype(const_cast<T*>(object)->one_ref_left(), TrueType {})
{
    const_cast<T*>(object)->one_ref_left();
    return {};
}

constexpr auto call_one_ref_left_if_present(...) -> FalseType
{
    return {};
}

class RefCountedBase {
    BASE_MAKE_NONCOPYABLE(RefCountedBase);
    BASE_MAKE_NONMOVABLE(RefCountedBase);

public:
    using RefCountType = unsigned int;
    using AllowOwnPtr = FalseType;

    void ref() const
    {
        auto old_ref_count = m_ref_count.fetch_add(1, Base::MemoryOrder::memory_order_relaxed);
        VERIFY(old_ref_count > 0);
        VERIFY(!Checked<RefCountType>::addition_would_overflow(old_ref_count, 1));
    }

    [[nodiscard]] bool try_ref() const
    {
        RefCountType expected = m_ref_count.load(Base::MemoryOrder::memory_order_relaxed);
        for (;;) {
            if (expected == 0)
                return false;
            VERIFY(!Checked<RefCountType>::addition_would_overflow(expected, 1));
            if (m_ref_count.compare_exchange_strong(expected, expected + 1, Base::MemoryOrder::memory_order_acquire))
                return true;
        }
    }

    [[nodiscard]] RefCountType ref_count() const
    {
        return m_ref_count.load(Base::MemoryOrder::memory_order_relaxed);
    }

protected:
    RefCountedBase() = default;
    ~RefCountedBase()
    {
        VERIFY(m_ref_count.load(Base::MemoryOrder::memory_order_relaxed) == 0);
    }

    RefCountType deref_base() const
    {
        auto old_ref_count = m_ref_count.fetch_sub(1, Base::MemoryOrder::memory_order_acq_rel);
        VERIFY(old_ref_count > 0);
        return old_ref_count - 1;
    }

    mutable Atomic<RefCountType> m_ref_count { 1 };
};



}