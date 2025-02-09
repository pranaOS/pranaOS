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
} // namespace Mods