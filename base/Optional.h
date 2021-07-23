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

    ALWAYS_INLINE Optional(const Optional& other)
#ifdef BASE_HAS_CONDITIONALLY_TRIVIAL
    requires(!IsTriviallyCopyConstructible<T>)
#endif
        : m_has_value(other.m_has_value)
    {
        if (other.has_value()) {
            new (&m_storage) T(other.value());
        }
    }

    ALWAYS_INLINE Optional(Optional&& other)
        : m_has_value(other.m_has_value)
    {
        if (other.has_value()) {
            new (&m_storage) T(other.release_value());
        }
    }

    template<typename U = T>
    ALWAYS_INLINE explicit(!IsConvertible<U&&, T>) Optional(U&& value) requires(!IsSame<RemoveCVReference<U>, Optional<T>> && IsConstructible<T, U&&>)
    : m_has_value(true)
    {
        new (&m_storage) T(forward<U>(value));
    }

    ALWAYS_INLINE Optional& operator=(const Optional& other)
#ifdef BASE_HAS_CONDITIONALLY_TRIVIAL
    requires(!IsTriviallyCopyConstructible<T> || !IsTriviallyDestructible<T>)
#endif
    {
        if (this != &other) {
            clear();
            m_has_value = other.m_has_value;
            if (other.has_value()) {
                new (&m_storage) T(other.value());
            }
        }
        return *this;
    }

};

}