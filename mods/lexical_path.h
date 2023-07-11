/**
 * @file lexical_path.h
 * @author Krisna Pranav
 * @brief lexical path
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

// #include <mods/string.h>
// #include <mods/vector.h>
#include "string.h"
#include "vector.h"

namespace Mods 
{
    class LexicalPath
    {

    public:
        LexicalPath() {}
        explicit LexicalPath(const StringView&);

        bool is_valid() const
        {
            return m_is_valid;
        };

        bool is_absolute() const
        {
            return m_is_absolute;
        }


        const String& string() const
        {
            return m_string;
        }

        const String& dirname() 
        {
            return m_dirname;
        }
    
    private:
        Vector<String> m_parts;
        String m_string;
        String m_dirname;
        String m_basename;
        String m_title;
        String m_extension;
        bool m_is_valid { false };
        bool m_is_absolute { false };
    };
}


using Mods::LexicalPath;
