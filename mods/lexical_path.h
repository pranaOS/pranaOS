/**
 * @file lexical_path.h
 * @author Krisna Pranav
 * @brief lexical path
 * @version 6.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "string.h"
#include "vector.h"


namespace Mods 
{

    class LexicalPath 
    {
    public:
        LexicalPath() { }
        explicit LexicalPath(const StringView&);

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_is_valid; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_absolute() const 
        { 
            return m_is_absolute; 
        }

        /**
         * @return const String& 
         */
        const String& string() const 
        { 
            return m_string; 
        }

        /**
         * @return const String& 
         */
        const String& dirname() const 
        { 
            return m_dirname; 
        }

        /**
         * @return const String& 
         */
        const String& basename() const 
        { 
            return m_basename; 
        }

        /**
         * @return const String& 
         */
        const String& title() const 
        { 
            return m_title; 
        }

        /**
         * @return const String& 
         */
        const String& extension() const 
        { 
            return m_extension; 
        }

        /**
         * @return const Vector<String>& 
         */
        const Vector<String>& parts() const 
        { 
            return m_parts; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_extension(const StringView&) const;

        /**
         * @return String 
         */
        static String canonicalized_path(const StringView&);

    private:
        void canonicalize();

        Vector<String> m_parts;
        String m_string;
        String m_dirname;
        String m_basename;
        String m_title;
        String m_extension;
        bool m_is_valid { false };
        bool m_is_absolute { false };
    };

    template<>
    struct Formatter<LexicalPath> : Formatter<StringView> {
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const LexicalPath& value)
        {
            Formatter<StringView>::format(params, builder, value.string());
        }
    };

};

using Mods::LexicalPath;
