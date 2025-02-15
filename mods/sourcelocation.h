/**
 * @file sourcelocation.h
 * @author Krisna Pranav
 * @brief Source Location
 * @version 6.0
 * @date 2025-02-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/stringview.h>
#include <mods/types.h>

namespace Mods
{
    class SourceLocation
    {
    public:
        /**
         * @return constexpr StringView 
         */
        [[nodiscard]] constexpr StringView function_name() const
        {
            return StringView(m_function);
        }

        /**
         * @return constexpr StringView 
         */
        [[nodiscard]] constexpr StringView filename() const
        {
            return StringView(m_file);
        }

        /**
         * @return constexpr u32 
         */
        [[nodiscard]] constexpr u32 line_number() const
        {
            return m_line;
        }   

        /**
         * @param file 
         * @param line 
         * @param function 
         * @return constexpr SourceLocation 
         */
        [[nodiscard]] static constexpr SourceLocation current(char const* const file = __builtin_FILE(), u32 line = __builtin_LINE(), char const* const function = __builtin_FUNCTION())
        {
            return SourceLocation(file, line, function);
        }   

        constexpr SourceLocation() = default;

    private:
        constexpr SourceLocation(char const* const file, u32 line, char const* const function)
            : m_function(function), m_file(file), m_line(line)
        {
        }

        char const* const m_function{nullptr};
        char const* const m_file{nullptr};
        const u32 m_line{0};
    }; // class SourceLocation
} // namespace Mods

template <>
struct Mods::Formatter<Mods::SourceLocation> : Mods::Formatter<FormatString>
{
    ErrorOr<void> format(FormatBuilder& builder, Mods::SourceLocation location)
    {
        return Mods::Formatter<FormatString>::format(builder, "[\x1b[34m{}\x1b[0m @ {}:{}]", location.function_name(), location.filename(), location.line_number());
    }
};

using Mods::SourceLocation;