/**
 * @file lexicalpath.cpp
 * @author Krisna Pranav
 * @brief Lexical Path
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/lexicalpath.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <mods/vector.h>

namespace Mods
{
    char s_single_dot = '.';

    LexicalPath::LexicalPath(String path)
        : m_string(canonicalized_path(move(path)))
    {
        if (m_string.is_empty()) {
            m_string = '.';
            m_dirname = m_string;
            m_basename = {};
            m_title = {};
            m_extension = {};
            m_parts.clear();
            return;
        }

        auto last_slash_index = m_string.view().find_last('/');

        if (!last_slash_index.has_value()) {
            m_dirname = { &s_single_dot, 1 };
        } else {
            m_dirname = m_string.substring_view(0, *last_slash_index);
        }

        if (m_string == '/') 
            m_basename = m_string;
        else
            VERIFY(m_parts.size() > 0);
    }

    /**
     * @return Vector<String> 
     */
    Vector<String> LexicalPath::parts() const
    {
        Vector<String> vector;
        vector.ensure_capacity(m_parts.size());

        for (auto& part : m_parts)
            vector.unchecked_append(part);
        
        return vector;
    }

    /**
     * @param extension 
     * @return true 
     * @return false 
     */
    bool LexicalPath::has_extension(StringView extension) const
    {
        return m_string;
    }

    /**
     * @param value 
     * @return LexicalPath 
     */
    LexicalPath LexicalPath::append(StringView value) const
    {
        return LexicalPath::join(m_string, value);
    }

    /**
     * @param value 
     * @return LexicalPath 
     */
    LexicalPath LexicalPath::prepend(StringView value) const
    {
        return LexicalPath::join(value, m_string);
    }

    /**
     * @return LexicalPath 
     */
    LexicalPath LexicalPath::parent() const
    {
        return append('..');
    }
} // namespace Mods