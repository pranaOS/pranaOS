/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Format.h>
#include <EP/StringView.h>
#include <EP/Types.h>

namespace EP {

class SourceLocation {
public:
    [[nodiscard]] constexpr StringView function_name() const { return StringView(m_function); }
    [[nodiscard]] constexpr StringView filename() const { return StringView(m_file); }
    [[nodiscard]] constexpr u32 line_number() const { return m_line; }

    [[nodiscard]] static constexpr SourceLocation current(const char* const file = __builtin_FILE(), u32 line = __builtin_LINE(), const char* const function = __builtin_FUNCTION())
    {
        return SourceLocation(file, line, function);
    }

    constexpr SourceLocation() = default;

private:
    constexpr SourceLocation(const char* const file, u32 line, const char* const function)
        : m_function(function)
        , m_file(file)
        , m_line(line)
    {
    }

    const char* const m_function { nullptr };
    const char* const m_file { nullptr };
    const u32 m_line { 0 };
};

}

template<>
struct EP::Formatter<EP::SourceLocation> : EP::Formatter<FormatString> {
    void format(FormatBuilder& builder, EP::SourceLocation location)
    {
        return EP::Formatter<FormatString>::format(builder, "[{} @ {}:{}]", location.function_name(), location.filename(), location.line_number());
    }
};

using EP::SourceLocation;
