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

    /**
     * @brief Construct a new Lexical Path:: Lexical Path object
     * 
     * @param path 
     */
    LexicalPath::LexicalPath(String path)
        : m_string(canonicalized_path(move(path)))
    {
        if(m_string.is_empty())
        {
            m_string = ".";
            m_dirname = m_string;
            m_basename = {};
            m_title = {};
            m_extension = {};
            m_parts.clear();
            return;
        }

        m_parts = m_string.split_view('/');

        auto last_slash_index = m_string.view().find_last('/');
        if(!last_slash_index.has_value())
        {
            m_dirname = {&s_single_dot, 1};
        }
        else if(*last_slash_index == 0)
        {
            // The path contains a single part and is absolute. m_dirname = "/"sv
            m_dirname = m_string.substring_view(0, 1);
        }
        else
        {
            m_dirname = m_string.substring_view(0, *last_slash_index);
        }

        if(m_string == "/")
            m_basename = m_string;
        else
        {
            VERIFY(m_parts.size() > 0);
            m_basename = m_parts.last();
        }

        auto last_dot_index = m_basename.find_last('.');
        
        if(last_dot_index.has_value() && *last_dot_index != 0)
        {
            m_title = m_basename.substring_view(0, *last_dot_index);
            m_extension = m_basename.substring_view(*last_dot_index + 1);
        }
        else
        {
            m_title = m_basename;
            m_extension = {};
        }
    }

    /**
     * @return Vector<String> 
     */
    Vector<String> LexicalPath::parts() const
    {
        Vector<String> vector;
        vector.ensure_capacity(m_parts.size());

        for(auto& part : m_parts)
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
        return m_string.ends_with(extension, CaseSensitivity::CaseInsensitive);
    }

    /**
     * @param path 
     * @return String 
     */
    String LexicalPath::canonicalized_path(String path)
    {
        if(path.is_null())
            return {};

        if(path.is_empty())
            return ".";

        if(!path.contains("."sv) && !path.contains("//"sv) && !path.ends_with('/'))
            return path;

        auto is_absolute = path[0] == '/';
        auto parts = path.split_view('/');
        size_t approximate_canonical_length = 0;
        Vector<String> canonical_parts;

        for(auto& part : parts)
        {
            if(part == ".")
                continue;
            if(part == "..")
            {
                if(canonical_parts.is_empty())
                {
                    if(is_absolute)
                    {
                        continue;
                    }
                }
                else
                {
                    if(canonical_parts.last() != "..")
                    {
                        canonical_parts.take_last();
                        continue;
                    }
                }
            }

            approximate_canonical_length += part.length() + 1;
            canonical_parts.append(part);
        }

        if(canonical_parts.is_empty() && !is_absolute)
            canonical_parts.append(".");

        StringBuilder builder(approximate_canonical_length);

        if(is_absolute)
            builder.append('/');

        builder.join('/', canonical_parts);
        return builder.to_string();
    }

    /**
     * @param dir_path 
     * @param target 
     * @return String 
     */
    String LexicalPath::absolute_path(String dir_path, String target)
    {
        if(LexicalPath(target).is_absolute())
        {
            return LexicalPath::canonicalized_path(target);
        }
        
        return LexicalPath::canonicalized_path(join(dir_path, target).string());
    }

    /**
     * @param a_path 
     * @param a_prefix 
     * @return String 
     */
    String LexicalPath::relative_path(StringView a_path, StringView a_prefix)
    {
        if(!a_path.starts_with('/') || !a_prefix.starts_with('/'))
        {
            return {};
        }

        if(a_path == a_prefix)
            return ".";

        auto path = canonicalized_path(a_path);
        auto prefix = canonicalized_path(a_prefix);

        if(path == prefix)
            return ".";

        if(prefix == "/"sv)
            return path.substring_view(1);

        if(path.starts_with(prefix) && path[prefix.length()] == '/')
        {
            return path.substring_view(prefix.length() + 1);
        }

        return path;
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
        return append("..");
    }

} // namespace Mods