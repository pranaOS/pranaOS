/**
 * @file genericlexer.cpp
 * @author Krisna Pranav
 * @brief genericlexer
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/assertions.h>
#include <mods/charactertypes.h>
#include <mods/genericlexer.h>
#include <mods/stringbuilder.h>

namespace Mods
{
    /**
     * @param count 
     * @return StringView 
     */
    StringView GenericLexer::consume(size_t count)
    {
        if (count == 0)
            return {};

        size_t start = m_index;
        size_t length = min(count, m_input.length() - m_index);
        m_index += length;

        return m_input.substring_view(start, length);
    }   

    /**
     * @return StringView 
     */
    StringView GenericLexer::consume_all()
    {
        if (is_eof())
            return {};
        
        auto rest = m_input.substring_view(m_index, m_input.length() - m_index);
        m_index = m_input.length();

        return rest;
    }

    /**
     * @return StringView 
     */
    StringView GenericLexer::consume_line()
    {
        size_t start = m_index;

        while (!is_eof() && peek() != '\r' && peek() != '\n')
            m_index++;
    
        size_t length = m_index - start;

        consume_specific('\r');
        consume_specific('\n');

        if (length == 0)
            return {};
        
        return m_input.substring_view(start, length);
    }
} // namespace Mods