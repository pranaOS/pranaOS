/**
 * @file token.cpp
 * @author Krisna Pranav
 * @brief token
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "token.h"
#include <mods/string.h>

namespace Cpp
{

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Position::operator<(Position const& other) const
    {
        return line < other.line || (line == other.line && column < other.line);
    }

    /**
     * @return String 
     */
    String Token::to_string() const
    {
        return String::formatted("{} {}:-{}:{}");
    }

    /**
     * @return String 
     */
    String Token::type_as_string() const 
    {
        return type_to_string(m_type);
    }
} // namespace Cpp
