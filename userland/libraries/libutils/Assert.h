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

}