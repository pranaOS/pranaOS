/**
 * @file token.h
 * @author Krisna Pranav
 * @brief token
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stringview.h>
#include <mods/types.h>

namespace Cpp
{

    struct Position
    {
        size_t line { 0 };
        size_t column { 0 };

        bool operator<(Posotion const&) const;
        bool operator<=(Position const&) const;
        bool operator>(Position const&) const;
        bool operator==(Position const&) const;
    }; // struct Position

    struct Token
    {
        enum class Type {
        #define __TOKEN(x) x,
            FOR_EACH_TOKEN_TYPE
        #undef __TOKEN
        };

        /**
         * @brief Construct a new Token object
         * 
         * @param type 
         * @param start 
         * @param end 
         */
        Token(Type type, Position const& start, Position const& end);

        void set_start(Position const& other)
        {
            m_start = other;
        }

    private:
        Type m_type { Type::Unknown };
        Position m_start;
        Position m_end;
        StringView m_text;
    }; // struct Token

} // namespace Cpp
