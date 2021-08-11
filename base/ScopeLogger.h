/*
 * Copyright (c) 2021, Denis Campredon <deni_@hotmail.fr>
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/SourceLocation.h>
#include <base/String.h>
#include <base/StringBuilder.h>

namespace Base {
template<bool = true>
class ScopeLogger {
public:
    ScopeLogger(StringView extra, const SourceLocation& location = SourceLocation::current())
        : m_location(location)
        , m_extra(extra)
    {
        StringBuilder sb;

        for (auto indent = m_depth++; indent > 0; indent--)
            sb.append(' ');
        if (m_extra.is_empty())
            dbgln("\033[1;{}m{}entering {}\033[0m", m_depth % 8 + 30, sb.to_string(), m_location);
        else
            dbgln("\033[1;{}m{}entering {}\033[0m ({})", m_depth % 8 + 30, sb.to_string(), m_location, m_extra);
    }

    ScopeLogger(SourceLocation location = SourceLocation::current())
        : ScopeLogger({}, move(location))
    {
    }

    ~ScopeLogger()
    {
        StringBuilder sb;

        auto depth = m_depth;
        for (auto indent = --m_depth; indent > 0; indent--)
            sb.append(' ');
        if (m_extra.is_empty())
            dbgln("\033[1;{}m{}leaving {}\033[0m", depth % 8 + 30, sb.to_string(), m_location);
        else
            dbgln("\033[1;{}m{}leaving {}\033[0m ({})", depth % 8 + 30, sb.to_string(), m_location, m_extra);
    }

private:
    static inline size_t m_depth = 0;
    SourceLocation m_location;
    String m_extra;
};

template<>
class ScopeLogger<false> {
public:
    template<typename... Args>
    ScopeLogger(Args...) { }
};

}

using Base::ScopeLogger;
