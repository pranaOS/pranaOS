/**
 * @file scopelogger.h
 * @author Krisna Pranav
 * @brief Scope Logger
 * @version 6.0
 * @date 2025-02-14
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/sourcelocation.h>
#include <mods/string.h>
#include <mods/stringbuilder.h>

namespace Mods
{
    /**
     * @tparam ool 
     */
    template <bool = true>
    class ScopeLogger
    {
    public:
        /**
         * @brief Construct a new ScopeLogger object
         * 
         * @param extra 
         * @param location 
         */
        ScopeLogger(StringView extra, SourceLocation const& location = SourceLocation::current())
            : m_location(location), m_extra(extra)
        {
            StringBuilder sb;

            for(auto indent = m_depth++; indent > 0; indent--)
                sb.append(' ');

            if(m_extra.is_empty())
                dbgln("\033[1;{}m{}entering {}\033[0m", m_depth % 8 + 30, sb.to_string(), m_location);
            else
                dbgln("\033[1;{}m{}entering {}\033[0m ({})", m_depth % 8 + 30, sb.to_string(), m_location, m_extra);
        }

        /**
         * @brief Construct a new ScopeLogger object
         * 
         * @param location 
         */
        ScopeLogger(SourceLocation location = SourceLocation::current())
            : ScopeLogger({}, move(location))
        {
        }

        /**
         * @brief Destroy the ScopeLogger object
         * 
         */
        ~ScopeLogger()
        {
            StringBuilder sb;

            auto depth = m_depth;
            for(auto indent = --m_depth; indent > 0; indent--)
                sb.append(' ');
            if(m_extra.is_empty())
                dbgln("\033[1;{}m{}leaving {}\033[0m", depth % 8 + 30, sb.to_string(), m_location);
            else
                dbgln("\033[1;{}m{}leaving {}\033[0m ({})", depth % 8 + 30, sb.to_string(), m_location, m_extra);
        }

    private:
        static inline size_t m_depth = 0;
        SourceLocation m_location;
        String m_extra;
    }; // class ScopeLogger

    /**
     * @tparam  
     */
    template <>
    class ScopeLogger<false>
    {
    public:
        /**
         * @brief Construct a new ScopeLogger object
         * 
         * @tparam Args 
         */
        template <typename... Args>
        ScopeLogger(Args...)
        {
        }
    }; // class ScopeLogger<false>
} // namespace Mods

using Mods::ScopeLogger;