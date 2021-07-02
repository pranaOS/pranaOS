/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <libio/Format.h>
#include <libutils/SourceLocation.h>

namespace Utils::Assert
{

template <typename A>
inline void not_null(const A a, SourceLocation location = SourceLocation::current())
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (a == nullptr)
        {
            assert_failed("variable is null", location.file(), location.function(), location.line());
        }
    }
}

inline void truth(bool a, SourceLocation location = SourceLocation::current())
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (!a)
        {
            assert_failed("variable is false", location.file(), location.function(), location.line());
        }
    }
}

inline void falsity(bool a, SourceLocation location = SourceLocation::current())
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (a)
        {
            assert_failed("variable is true", location.file(), location.function(), location.line());
        }
    }
}

template <typename A, typename B>
inline void equal(const A a, const B b, SourceLocation location = SourceLocation::current())
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (a != static_cast<A>(b))
        {
            assert_failed(IO::format("{} not equal {}", a, b).cstring(), location.file(), location.function(), location.line());
        }
    }
}

template <typename A, typename B>
inline void not_equal(const A a, const B b, SourceLocation location = SourceLocation::current())
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (a == static_cast<A>(b))
        {
            assert_failed(IO::format("{} equals {}", a, b).cstring(), location.file(), location.function(), location.line());
        }
    }
}

}