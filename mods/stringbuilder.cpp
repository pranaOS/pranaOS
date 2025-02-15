/**
 * @file stringbuilder.cpp
 * @author Krisna Pranav
 * @brief string builder
 * @version 6.0
 * @date 2025-02-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/bytebuffer.h>
#include <mods/checked.h>
#include <mods/printfimplementation.h>
#include <mods/stdlibextra.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <mods/unicodeutils.h>
#include <mods/utf32view.h>

#ifndef KERNEL
#include <mods/string.h>
#include <mods/utf16view.h>
#endif

namespace Mods
{
    /**
     * @param size
     * @return ErrorOr<void>
     */
    inline ErrorOr<void> StringBuilder::will_append(size_t size)
    {
        Checked<size_t> needed_capacity = m_buffer.size();
        needed_capacity += size;
        VERIFY(!needed_capacity.has_overflow());
        
        if(needed_capacity <= m_buffer.capacity())
            return {};

        Checked<size_t> expanded_capacity = needed_capacity;
        expanded_capacity *= 2;
        VERIFY(!expanded_capacity.has_overflow());
        TRY(m_buffer.try_ensure_capacity(expanded_capacity.value()));
        return {};
    }

    /**
     * @brief Construct a new String Builder:: String Builder object
     * 
     * @param initial_capacity 
     */
    StringBuilder::StringBuilder(size_t initial_capacity)
    {
        m_buffer.ensure_capacity(initial_capacity);
    }

    /**
     * @param string 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append(StringView string)
    {
        if(string.is_empty())
            return {};
        TRY(will_append(string.length()));
        TRY(m_buffer.try_append(string.characters_without_null_termination(), string.length()));
        return {};
    }

    /**
     * @param ch 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append(char ch)
    {
        TRY(will_append(1));
        TRY(m_buffer.try_append(ch));
        return {};
    }

    /**
     * @param string 
     */
    void StringBuilder::append(StringView string)
    {
        MUST(try_append(string));
    }

    /**
     * @param characters 
     * @param length 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append(char const* characters, size_t length)
    {
        return try_append(StringView{characters, length});
    }

    /**
     * @param characters 
     * @param length 
     */
    void StringBuilder::append(char const* characters, size_t length)
    {
        MUST(try_append(characters, length));
    }
    
    /**
     * @param ch 
     */
    void StringBuilder::append(char ch)
    {
        MUST(try_append(ch));
    }

    /**
     * @param fmt 
     * @param ap 
     */
    void StringBuilder::appendvf(char const* fmt, va_list ap)
    {
        printf_internal([this](char*&, char ch)
                        { append(ch); },
                        nullptr,
                        fmt,
                        ap);
    }

    /**
     * @return ByteBuffer 
     */
    ByteBuffer StringBuilder::to_byte_buffer() const
    {
        return ByteBuffer::copy(data(), length()).release_value_but_fixme_should_propagate_errors();
    }

    #ifndef KERNEL
    String StringBuilder::to_string() const
    {
        if(is_empty())
            return String::empty();
        return String((char const*)data(), length());
    }

    String StringBuilder::build() const
    {
        return to_string();
    }
    #endif

    /**
     * @return StringView 
     */
    StringView StringBuilder::string_view() const
    {
        return StringView{data(), m_buffer.size()};
    }

    void StringBuilder::clear()
    {
        m_buffer.clear();
    }

    /**
     * @param code_point 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append_code_point(u32 code_point)
    {
        auto nwritten = Mods::UnicodeUtils::code_point_to_utf8(code_point, [this](char c)
                                                            { append(c); });
        if(nwritten < 0)
        {
            TRY(try_append(0xef));
            TRY(try_append(0xbf));
            TRY(try_append(0xbd));
        }
        return {};
    }

    /**
     * @param code_point 
     */
    void StringBuilder::append_code_point(u32 code_point)
    {
        MUST(try_append_code_point(code_point));
    }

    #ifndef KERNEL
    ErrorOr<void> StringBuilder::try_append(Utf16View const& utf16_view)
    {
        for(size_t i = 0; i < utf16_view.length_in_code_units();)
        {
            auto code_point = utf16_view.code_point_at(i);
            TRY(try_append_code_point(code_point));

            i += (code_point > 0xffff ? 2 : 1);
        }
        return {};
    }

    /**
     * @param utf16_view 
     */
    void StringBuilder::append(Utf16View const& utf16_view)
    {
        MUST(try_append(utf16_view));
    }
    #endif

    /**
     * @param utf32_view 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append(Utf32View const& utf32_view)
    {
        for(size_t i = 0; i < utf32_view.length(); ++i)
        {
            auto code_point = utf32_view.code_points()[i];
            TRY(try_append_code_point(code_point));
        }
        return {};
    }

    /**
     * @param utf32_view 
     */
    void StringBuilder::append(Utf32View const& utf32_view)
    {
        MUST(try_append(utf32_view));
    }   

    /**
     * @param ch 
     */
    void StringBuilder::append_as_lowercase(char ch)
    {
        if(ch >= 'A' && ch <= 'Z')
            append(ch + 0x20);
        else
            append(ch);
    }   

    /**
     * @param string 
     */
    void StringBuilder::append_escaped_for_json(StringView string)
    {
        MUST(try_append_escaped_for_json(string));
    }

    /**
     * @param string 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append_escaped_for_json(StringView string)
    {
        for(auto ch : string)
        {
            switch(ch)
            {
            case '\b':
                TRY(try_append("\\b"));
                break;
            case '\n':
                TRY(try_append("\\n"));
                break;
            case '\t':
                TRY(try_append("\\t"));
                break;
            case '\"':
                TRY(try_append("\\\""));
                break;
            case '\\':
                TRY(try_append("\\\\"));
                break;
            default:
                if(ch >= 0 && ch <= 0x1f)
                    TRY(try_appendff("\\u{:04x}", ch));
                else
                    TRY(try_append(ch));
            }
        }
        return {};
    }
} // namespace Mods