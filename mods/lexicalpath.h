/**
 * @file lexical_path.h
 * @author Krisna Pranav
 * @brief lexical path
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>
#include <mods/vector.h>

namespace Mods
{
    class LexicalPath
    {
    public:
        /**
         * @brief Construct a new Lexical Path object
         * 
         */
        explicit LexicalPath(String);

        /**
         * @return true 
         * @return false 
         */
        bool is_absolute() const
        {
            return !m_string.is_empty() && m_string[0] == '/';
        }

        /**
         * @return String const& 
         */
        String const& string() const
        {
            return m_string;
        }

        /**
         * @return StringView 
         */
        StringView dirname() const
        {
            return m_dirname;
        }

        /**
         * @return StringView 
         */
        StringView basename() const
        {
            return m_basename;
        }

        /**
         * @return StringView 
         */
        StringView title() const
        {
            return m_title;
        }

        /**
         * @return StringView 
         */
        StringView extension() const
        {
            return m_extension;
        }

        /**
         * @return Vector<StringView> const& 
         */
        Vector<StringView> const& parts_view() const
        {
            return m_parts;
        }

        [[nodiscard]] Vector<String> parts() const;

        bool has_extension(StringView) const;

        /**
         * @return LexicalPath 
         */
        [[nodiscard]] LexicalPath append(StringView) const;
        [[nodiscard]] LexicalPath prepend(StringView) const;
        [[nodiscard]] LexicalPath parent() const;

        /**
         * @return String 
         */
        [[nodiscard]] static String canonicalized_path(String);

        /**
         * @param dir_path 
         * @param target 
         * @return String 
         */
        [[nodiscard]] static String absolute_path(String dir_path, String target);

        /**
         * @param absolute_path 
         * @param prefix 
         * @return String 
         */
        [[nodiscard]] static String relative_path(StringView absolute_path, StringView prefix);

        /**
         * @tparam S 
         * @param first 
         * @param rest 
         * @return LexicalPath 
         */
        template <typename... S>
        [[nodiscard]] static LexicalPath join(StringView first, S&&... rest)
        {
            StringBuilder builder;
            builder.append(first);

            ((builder.append('/'), builder.append(forward<S>(rest))), ...);

            return LexicalPath{builder.to_string()};
        }

        /**
         * @param path 
         * @return String 
         */
        [[nodiscard]] static String dirname(String path)
        {
            auto lexical_path = LexicalPath(move(path));
            return lexical_path.dirname();
        }

        /**
         * @param path 
         * @return String 
         */
        [[nodiscard]] static String basename(String path)
        {
            auto lexical_path = LexicalPath(move(path));
            return lexical_path.basename();
        }

        /**
         * @param path 
         * @return String 
         */
        [[nodiscard]] static String title(String path)
        {
            auto lexical_path = LexicalPath(move(path));
            return lexical_path.title();
        }

        /**
         * @param path 
         * @return String 
         */
        [[nodiscard]] static String extension(String path)
        {
            auto lexical_path = LexicalPath(move(path));
            return lexical_path.extension();
        }

    private:
        Vector<StringView> m_parts;
        String m_string;
        StringView m_dirname;
        StringView m_basename;
        StringView m_title;
        StringView m_extension;
    }; // class LexicalPath

    /**
     * @tparam  
     */
    template <>
    struct Formatter<LexicalPath> : Formatter<StringView>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, LexicalPath const& value)
        {
            return Formatter<StringView>::format(builder, value.string());
        }
    }; // struct Formatter<LexicalPath> : Formatter<StringView>

}; // namespace Mods

using Mods::LexicalPath;
