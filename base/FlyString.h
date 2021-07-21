/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "base/StringUtils.h"
#include <base/String.h>

namespace Base {

class FlyString {
public:
    FlyString() = default;
    FlyString(const FlyString& other)
        : m_impl(other.impl())
    {
    }

    FlyString(FlyString&& other)
        : m_impl(move(other.m_impl))
    {
    }

    FlyString(const String&);
    FlyString(const StringView&);
    FlyString(const char* string)
        : FlyString(static_cast<String>(string))
    {
    }

    static FlyString from_fly_impl(NonnullRefPtr<StringImpl> impl)
    {
        VERIFY(impl->is_fly());
        FlyString string;
        string.m_impl = move(impl);
        return string;
    }

    FlyString& operator=(const FlyString& other)
    {
        m_impl = other.m_impl;
        return *this;
    }

    FlyString& operator=(FlyString&& other)
    {
        m_impl = move(other.m_impl);
        return *this;
    }
}

}