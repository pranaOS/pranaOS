/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StdLibExtras.h>
#include <kernel/locking/SpinLock.h>

namespace Kernel {

template<typename T>
class SpinLockLockedResource {
    BASE_MAKE_NONCOPYABLE(SpinLockLockedResource);

public:
    SpinLockLockedResource(T* value, RecursiveSpinLock& spinlock)
        : m_value(value)
        , m_scoped_spinlock(spinlock)
    {
    }

    ALWAYS_INLINE T const* operator->() const { return m_value; }
    ALWAYS_INLINE T const& operator*() const { return *m_value; }

    ALWAYS_INLINE T* operator->() { return m_value; }
    ALWAYS_INLINE T& operator*() { return *m_value; }

    ALWAYS_INLINE T const* get() const { return m_value; }
    ALWAYS_INLINE T* get() { return m_value; }

private:
    T* m_value;
    ScopedSpinLock<RecursiveSpinLock> m_scoped_spinlock;
};

class SpinLockContendedResource {
    template<typename>
    friend class SpinLockLockedResource;

    BASE_MAKE_NONCOPYABLE(SpinLockContendedResource);
    BASE_MAKE_NONMOVABLE(SpinLockContendedResource);

public:
    SpinLockContendedResource() = default;

protected:
    mutable RecursiveSpinLock m_spinlock;
};

}
