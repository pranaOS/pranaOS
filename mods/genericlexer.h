/**
 * @file genericlexer.h
 * @author Krisna Pranav
 * @brief genericlexer
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/result.h>
#include <mods/stringview.h>

namespace Mods
{
    class GenericLexer
    {
    public:
        /**
         * @return input
         * 
         */
        constexpr explicit GenericLexer(StringView input)
            : m_input(input)
        {
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t tell() const
        {
            return m_index;
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t tell_remaining() const
        {
            return m_input.length() - m_index;
        }

        /**
         * @return StringView 
         */
        StringView remaining() const
        {
            return m_input.substring_view(m_index);
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_eof() const
        {
            return m_index >= m_input.length();
        }

        /**
         * @param offset 
         * @return constexpr char 
         */
        constexpr char peek(size_t offset = 0) const
        {
            return (m_index + offset < m_input.length()) ? m_input[m_index + offset] : '\0';
        }

        /**
         * @param expected 
         * @return true 
         * @return false 
         */
        constexpr bool next_is(char expected) const
        {
            return peek() == expected;
        }

        /**
         * @param expected 
         * @return true 
         * @return false 
         */
        constexpr bool next_is(StringView expected) const
        {
            for(size_t i = 0; i < expected.length(); ++i)
                if(peek(i) != expected[i])
                    return false;
            return true;
        }
        
        /**
         * @param expected 
         * @return true 
         * @return false 
         */
        constexpr bool next_is(char const* expected) const
        {
            for(size_t i = 0; expected[i] != '\0'; ++i)
                if(peek(i) != expected[i])
                    return false;

            return true;
        }

        constexpr void retreat()
        {
            VERIFY(m_index > 0);
            --m_index;
        }

        /**
         * @param count 
         */
        constexpr void retreat(size_t count)
        {
            VERIFY(m_index >= count);
            m_index -= count;
        }

        constexpr char consume()
        {
            VERIFY(!is_eof());
            return m_input[m_index++];
        }

        /**
         * @tparam T 
         * @param next 
         * @return true 
         * @return false 
         */
        template <typename T>
        constexpr bool consume_specific(const T& next)
        {
            if(!next_is(next))
                return false;

            if constexpr(requires { next.length(); })
            {
                ignore(next.length());
            }
            else
            {
                ignore(sizeof(next));
            }
            return true;
        }

    #ifndef KERNEL
        bool consume_specific(String const& next)
        {
            return consume_specific(StringView{next});
        }
    #endif

        /**
         * @param next 
         * @return true 
         * @return false 
         */
        constexpr bool consume_specific(char const* next)
        {
            return consume_specific(StringView{next});
        }

        constexpr char consume_escaped_character(char escape_char = '\\', StringView escape_map = "n\nr\rt\tb\bf\f")
        {
            if(!consume_specific(escape_char))
                return consume();

            auto c = consume();

            for(size_t i = 0; i < escape_map.length(); i += 2)
            {
                if(c == escape_map[i])
                    return escape_map[i + 1];
            }

            return c;
        }
        
        /**
         * @param count 
         * @return StringView 
         */
        StringView consume(size_t count);

        /**
         * @return StringView 
         */
        StringView consume_all();

        /**
         * @return StringView 
         */
        StringView consume_line();

        /**
         * @return StringView 
         */
        StringView consume_until(char);

        /**
         * @return StringView 
         */
        StringView consume_until(char const*);

        /**
         * @return StringView 
         */
        StringView consume_until(StringView);

        /**
         * @param escape_char 
         * @return StringView 
         */
        StringView consume_quoted_string(char escape_char = 0);
    #ifndef KERNEL
        String consume_and_unescape_string(char escape_char = '\\');
    #endif

        enum class UnicodeEscapeError
        {
            MalformedUnicodeEscape,
            UnicodeEscapeOverflow,
        }; // enum class UnicodeEscapeError

    #ifndef KERNEL
        Result<u32, UnicodeEscapeError> consume_escaped_code_point(bool combine_surrogate_pairs = true);
    #endif
        
        /**
         * @brief 
         * 
         * @param count 
         */
        constexpr void ignore(size_t count = 1)
        {
            count = min(count, m_input.length() - m_index);
            m_index += count;
        }

        /**
         * @param stop 
         */
        constexpr void ignore_until(char stop)
        {
            while(!is_eof() && peek() != stop)
            {
                ++m_index;
            }
            ignore();
        }

        /**
         * @param stop 
         */
        constexpr void ignore_until(char const* stop)
        {
            while(!is_eof() && !next_is(stop))
            {
                ++m_index;
            }
            ignore(__builtin_strlen(stop));
        }

        /**
         * @tparam TPredicate 
         * @param pred 
         * @return true 
         * @return false 
         */
        template <typename TPredicate>
        constexpr bool next_is(TPredicate pred) const
        {
            return pred(peek());
        }

        /**
         * @tparam TPredicate 
         * @param pred 
         * @return StringView 
         */
        template <typename TPredicate>
        StringView consume_while(TPredicate pred)
        {
            size_t start = m_index;
            while(!is_eof() && pred(peek()))
                ++m_index;
            size_t length = m_index - start;

            if(length == 0)
                return {};
            return m_input.substring_view(start, length);
        }

        /**
         * @tparam TPredicate 
         * @param pred 
         * @return StringView 
         */
        template <typename TPredicate>
        StringView consume_until(TPredicate pred)
        {
            size_t start = m_index;
            while(!is_eof() && !pred(peek()))
                ++m_index;
            size_t length = m_index - start;

            if(length == 0)
                return {};
            return m_input.substring_view(start, length);
        }

        /**
         * @tparam TPredicate 
         * @param pred 
         */
        template <typename TPredicate>
        constexpr void ignore_while(TPredicate pred)
        {
            while(!is_eof() && pred(peek()))
                ++m_index;
        }

        /**
         * @tparam TPredicate 
         * @param pred 
         */
        template <typename TPredicate>
        constexpr void ignore_until(TPredicate pred)
        {
            while(!is_eof() && !pred(peek()))
                ++m_index;
        }

    protected:
        StringView m_input;
        size_t m_index{0};

    private:
    #ifndef KERNEL
        Result<u32, UnicodeEscapeError> decode_code_point();
        Result<u32, UnicodeEscapeError> decode_single_or_paired_surrogate(bool combine_surrogate_pairs);
    #endif
    };

    /**
     * @param values 
     * @return constexpr auto 
     */
    constexpr auto is_any_of(StringView values)
    {
        return [values](auto c)
        { return values.contains(c); };
    }

    /**
     * @param values 
     * @return constexpr auto 
     */
    constexpr auto is_not_any_of(StringView values)
    {
        return [values](auto c)
        { return !values.contains(c); };
    }

    constexpr auto is_path_separator = is_any_of("/\\");
    constexpr auto is_quote = is_any_of("'\"");

} // namespace Mods

using Mods::GenericLexer;
using Mods::is_any_of;
using Mods::is_path_separator;
using Mods::is_quote;
