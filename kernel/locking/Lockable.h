/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/locking/Mutex.h>

namespace Kernel {

template<typename T>
class Lockable {
public:
    Lockable() = default;
    Lockable(T&& resource)
        : m_resource(move(resource))
    {
    }
    [[nodiscard]] Mutex& lock() { return m_lock; }
    [[nodiscard]] T& resource() { return m_resource; }

    [[nodiscard]] T lock_and_copy()
    {
        MutexLocker locker(m_lock);
        return m_resource;
    }

private:
    T m_resource;
    Mutex m_lock;
};

}
