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
    private:
        constexpr SourceLocation(char const* const file, u32 line, char* function)
            : m_function(function)
            , m_file(file)
            , m_line(line)
        {}

        char const* const m_function { nullptr };
        char const* const m_file { nullptr };
        const u32 m_line { 0 };
    };
}

using Mods::SourceLocation;