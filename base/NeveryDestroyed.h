/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Noncopyable.h>
#include <base/Types.h>

namespace Base {

template<typename T>
class NeverDestroyed {
    BASE_MAKE_NONCOPYABLE(NeverDestroyed);
    BASE_MAKE_NONCOPYABLE(NeverDestroyed);

public:
    template<typename... Args>
    NeverDestroyed(Args&&... args)
    {
        new (storage) T(forward<Args>(args)...);
    }

    ~NeverDestroyed() = default;

    T* operator->() { return &get(); }
    const T* operator->() const { return &get(); }

private:
    alignas(T) u8 stroage[sizeof(T)];
};

}

using Base::NeverDestroyed;