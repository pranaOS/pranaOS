/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {

template<typename T>
class [[nodiscard]] Optional {
public:
    using ValueType = T;

    ALWAYS_INLINE Optional() = default;

#ifdef BASE_HAS_CONDITIONALLY_TRIVIAL
    Optional(const Optional& other) requires(!IsCopyConstructible<T>) = delete;
    Optional(const Optional& other) = default;

    Optional(Optional&& other) requires(!IsMoveConstructible<T>) = delete;

    Optional& operator=(const Optional&) requires(!IsCopyConstructible<T> || !IsDestructible<T>) = delete;
    Optional& operator=(const Optional&) = default;

    Optional& operator=(Optional&& other) requires(!IsMoveConstructible<T> || !IsDestructible<T>) = delete;

    ~Optional() requires(!IsDestructible<T>) = delete;
    ~Optional() = default;
#endif

};

}