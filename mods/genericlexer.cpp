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

#ifndef KERNEL
#include <mods/string.h>
#include <mods/utf16view.h>
#endif

namespace Mods
{
    /**
     * @param count 
     * @return StringView 
     */
    StringView GenericLexer::consume(size_t count)
    {
        if(count == 0)
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
        if(is_eof())
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
        while(!is_eof() && peek() != '\r' && peek() != '\n')
            m_index++;
        size_t length = m_index - start;

        consume_specific('\r');
        consume_specific('\n');

        if(length == 0)
            return {};
        return m_input.substring_view(start, length);
    }

    /**
     * @param stop 
     * @return StringView 
     */
    StringView GenericLexer::consume_until(char stop)
    {
        size_t start = m_index;
        while(!is_eof() && peek() != stop)
            m_index++;
        size_t length = m_index - start;

        if(length == 0)
            return {};
        return m_input.substring_view(start, length);
    }

    /**
     * @param stop 
     * @return StringView 
     */
    StringView GenericLexer::consume_until(char const* stop)
    {
        size_t start = m_index;
        while(!is_eof() && !next_is(stop))
            m_index++;
        size_t length = m_index - start;

        if(length == 0)
            return {};
        return m_input.substring_view(start, length);
    }

    /**
     * @param stop 
     * @return StringView 
     */
    StringView GenericLexer::consume_until(StringView stop)
    {
        size_t start = m_index;
        while(!is_eof() && !next_is(stop))
            m_index++;
        size_t length = m_index - start;

        if(length == 0)
            return {};
        return m_input.substring_view(start, length);
    }

    /**
     * @param escape_char 
     * @return StringView 
     */
    StringView GenericLexer::consume_quoted_string(char escape_char)
    {
        if(!next_is(is_quote))
            return {};

        char quote_char = consume();
        size_t start = m_index;

        while(!is_eof())
        {
            if(next_is(escape_char))
                m_index++;
            else if(next_is(quote_char))
                break;
            m_index++;
        }

        size_t length = m_index - start;

        if(peek() != quote_char)
        {
            m_index = start - 1;
            return {};
        }

        ignore();

        return m_input.substring_view(start, length);
    }

    #ifndef KERNEL
    /**
     * @param escape_char 
     * @return String 
     */
    String GenericLexer::consume_and_unescape_string(char escape_char)
    {
        auto view = consume_quoted_string(escape_char);
        if(view.is_null())
            return {};

        StringBuilder builder;
        for(size_t i = 0; i < view.length(); ++i)
            builder.append(consume_escaped_character(escape_char));
        return builder.to_string();
    }

    /**
     * @param combine_surrogate_pairs 
     * @return Result<u32, UnicodeEscapeError> 
     */
    auto GenericLexer::consume_escaped_code_point(bool combine_surrogate_pairs) -> Result<u32, UnicodeEscapeError>
    {
        if(!consume_specific("\\u"sv))
            return UnicodeEscapeError::MalformedUnicodeEscape;

        if(next_is('{'))
            return decode_code_point();
        return decode_single_or_paired_surrogate(combine_surrogate_pairs);
    }

    /**
     * @return Result<u32, UnicodeEscapeError> 
     */
    auto GenericLexer::decode_code_point() -> Result<u32, UnicodeEscapeError>
    {
        bool starts_with_open_bracket = consume_specific('{');
        VERIFY(starts_with_open_bracket);

        u32 code_point = 0;

        while(true)
        {
            if(!next_is(is_ascii_hex_digit))
                return UnicodeEscapeError::MalformedUnicodeEscape;

            auto new_code_point = (code_point << 4u) | parse_ascii_hex_digit(consume());
            if(new_code_point < code_point)
                return UnicodeEscapeError::UnicodeEscapeOverflow;

            code_point = new_code_point;
            if(consume_specific('}'))
                break;
        }

        if(is_unicode(code_point))
            return code_point;
        return UnicodeEscapeError::UnicodeEscapeOverflow;
    }

    /**
     * @param combine_surrogate_pairs 
     * @return Result<u32, UnicodeEscapeError> 
     */
    auto GenericLexer::decode_single_or_paired_surrogate(bool combine_surrogate_pairs) -> Result<u32, UnicodeEscapeError>
    {
        constexpr size_t surrogate_length = 4;

        auto decode_one_surrogate = [&]() -> Optional<u16>
        {
            u16 surrogate = 0;

            for(size_t i = 0; i < surrogate_length; ++i)
            {
                if(!next_is(is_ascii_hex_digit))
                    return {};

                surrogate = (surrogate << 4u) | parse_ascii_hex_digit(consume());
            }

            return surrogate;
        };

        auto high_surrogate = decode_one_surrogate();

        if(!high_surrogate.has_value())
            return UnicodeEscapeError::MalformedUnicodeEscape;
        if(!Utf16View::is_high_surrogate(*high_surrogate))
            return *high_surrogate;
        if(!combine_surrogate_pairs || !consume_specific("\\u"sv))
            return *high_surrogate;

        auto low_surrogate = decode_one_surrogate();

        if(!low_surrogate.has_value())
            return UnicodeEscapeError::MalformedUnicodeEscape;
        if(Utf16View::is_low_surrogate(*low_surrogate))
            return Utf16View::decode_surrogate_pair(*high_surrogate, *low_surrogate);

        retreat(6);
        return *high_surrogate;
    }
    #endif
} // namespace Mods
