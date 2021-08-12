/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StdLibExtras.h>
#include <kernel/locking/Mutex.h>

namespace Kernel {

template<typename T, LockMode LockingMode>
class LockedResource {
    BASE_MAKE_NONCOPYABLE(LockedResource);

public:
    LockedResource(T* value, Mutex& mutex)
        : m_value(value)
        , m_mutex_locker(mutex, LockingMode)
    {
    }

    ALWAYS_INLINE T const* operator->() const { return m_value; }
    ALWAYS_INLINE T const& operator*() const { return *m_value; }

    ALWAYS_INLINE T* operator->() requires(!IsConst<T>) { return m_value; }
    ALWAYS_INLINE T& operator*() requires(!IsConst<T>) { return *m_value; }

    ALWAYS_INLINE T const* get() const { return m_value; }
    ALWAYS_INLINE T* get() requires(!IsConst<T>) { return m_value; }

private:
    T* m_value;
    MutexLocker m_mutex_locker;
};

class ContendedResource {
    template<typename, LockMode>
    friend class LockedResource;

    BASE_MAKE_NONCOPYABLE(ContendedResource);
    BASE_MAKE_NONMOVABLE(ContendedResource);

public:
    ContendedResource() = default;

protected:
    mutable Mutex m_mutex;
};

}
