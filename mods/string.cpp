/**
 * @file string.cpp
 * @author Krisna Pranav
 * @brief string
 * @version 6.0
 * @date 2025-02-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/bytebuffer.h>
#include <mods/flystring.h>
#include <mods/format.h>
#include <mods/function.h>
#include <mods/memory.h>
#include <mods/stdlibextra.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @param fly_string 
     * @return true 
     * @return false 
     */
    bool String::operator==(FlyString const& fly_string) const
    {
        return m_impl == fly_string.impl() || view() == fly_string.view();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator==(String const& other) const
    {
        return m_impl == other.impl() || view() == other.view();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator==(StringView other) const
    {
        return view() == other;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator<(String const& other) const
    {
        return view() < other.view();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator>(String const& other) const
    {
        return view() > other.view();
    }

    /**
     * @param buffer 
     * @param buffer_size 
     * @return true 
     * @return false 
     */
    bool String::copy_characters_to_buffer(char* buffer, size_t buffer_size) const
    {
        VERIFY(buffer_size > 0);

        size_t characters_to_copy = min(length(), buffer_size - 1);
        __builtin_memcpy(buffer, characters(), characters_to_copy);
        buffer[characters_to_copy] = 0;

        return characters_to_copy == length();
    }

    /**
     * @return String 
     */
    String String::isolated_copy() const
    {
        if(!m_impl)
            return {};
        if(!m_impl->length())
            return empty();

        char* buffer;
        auto impl = StringImpl::create_uninitialized(length(), buffer);
        memcpy(buffer, m_impl->characters(), m_impl->length());
        return String(move(*impl));
    }

    /**
     * @param start 
     * @param length 
     * @return String 
     */
    String String::substring(size_t start, size_t length) const
    {
        if(!length)
            return String::empty();

        VERIFY(m_impl);
        VERIFY(!Checked<size_t>::addition_would_overflow(start, length));
        VERIFY(start + length <= m_impl->length());
        return {characters() + start, length};
    }

    /**
     * @param start 
     * @return String 
     */
    String String::substring(size_t start) const
    {
        VERIFY(m_impl);
        VERIFY(start <= length());
        return {characters() + start, length() - start};
    }

    /**
     * @param start 
     * @param length 
     * @return StringView 
     */
    StringView String::substring_view(size_t start, size_t length) const
    {
        VERIFY(m_impl);
        VERIFY(!Checked<size_t>::addition_would_overflow(start, length));
        VERIFY(start + length <= m_impl->length());
        return {characters() + start, length};
    }

    /**
     * @param start 
     * @return StringView 
     */
    StringView String::substring_view(size_t start) const
    {
        VERIFY(m_impl);
        VERIFY(start <= length());
        return {characters() + start, length() - start};
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<String> 
     */
    Vector<String> String::split(char separator, bool keep_empty) const
    {
        return split_limit(separator, 0, keep_empty);
    }

    /**
     * @param separator 
     * @param limit 
     * @param keep_empty 
     * @return Vector<String> 
     */
    Vector<String> String::split_limit(char separator, size_t limit, bool keep_empty) const
    {
        if(is_empty())
            return {};

        Vector<String> v;
        size_t substart = 0;

        for(size_t i = 0; i < length() && (v.size() + 1) != limit; ++i)
        {
            char ch = characters()[i];
            if(ch == separator)
            {
                size_t sublen = i - substart;
                if(sublen != 0 || keep_empty)
                    v.append(substring(substart, sublen));
                substart = i + 1;
            }
        }
        size_t taillen = length() - substart;
        if(taillen != 0 || keep_empty)
            v.append(substring(substart, taillen));
        return v;
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> String::split_view(Function<bool(char)> separator, bool keep_empty) const
    {
        if(is_empty())
            return {};

        Vector<StringView> v;
        size_t substart = 0;

        for(size_t i = 0; i < length(); ++i)
        {
            char ch = characters()[i];
            if(separator(ch))
            {
                size_t sublen = i - substart;
                if(sublen != 0 || keep_empty)
                    v.append(substring_view(substart, sublen));
                substart = i + 1;
            }
        }

        size_t taillen = length() - substart;
        if(taillen != 0 || keep_empty)
            v.append(substring_view(substart, taillen));
        return v;
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> String::split_view(char const separator, bool keep_empty) const
    {
        return split_view([separator](char ch)
                        { return ch == separator; },
                        keep_empty);
    }

    /**
     * @return ByteBuffer 
     */
    ByteBuffer String::to_byte_buffer() const
    {
        if(!m_impl)
            return {};
        
        return ByteBuffer::copy(bytes()).release_value_but_fixme_should_propagate_errors();
    }

    /**
     * @tparam T 
     * @param trim_whitespace 
     * @return Optional<T> 
     */
    template <typename T>
    Optional<T> String::to_int(TrimWhitespace trim_whitespace) const
    {
        return StringUtils::convert_to_int<T>(view(), trim_whitespace);
    }

    template Optional<i8> String::to_int(TrimWhitespace) const;
    template Optional<i16> String::to_int(TrimWhitespace) const;
    template Optional<i32> String::to_int(TrimWhitespace) const;
    template Optional<i64> String::to_int(TrimWhitespace) const;

    /**
     * @tparam T 
     * @param trim_whitespace 
     * @return Optional<T> 
     */
    template <typename T>
    Optional<T> String::to_uint(TrimWhitespace trim_whitespace) const
    {
        return StringUtils::convert_to_uint<T>(view(), trim_whitespace);
    }

    template Optional<u8> String::to_uint(TrimWhitespace) const;
    template Optional<u16> String::to_uint(TrimWhitespace) const;
    template Optional<u32> String::to_uint(TrimWhitespace) const;
    template Optional<unsigned long> String::to_uint(TrimWhitespace) const;
    template Optional<unsigned long long> String::to_uint(TrimWhitespace) const;

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::starts_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::starts_with(*this, str, case_sensitivity);
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    bool String::starts_with(char ch) const
    {
        if(is_empty())
            return false;
        return characters()[0] == ch;
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::ends_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::ends_with(*this, str, case_sensitivity);
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    bool String::ends_with(char ch) const
    {
        if(is_empty())
            return false;
        return characters()[length() - 1] == ch;
    }

    /**
     * @param ch 
     * @param count 
     * @return String 
     */
    String String::repeated(char ch, size_t count)
    {
        if(!count)
            return empty();

        char* buffer;
        auto impl = StringImpl::create_uninitialized(count, buffer);
        memset(buffer, ch, count);
        return *impl;
    }

    /**
     * @param string 
     * @param count 
     * @return String 
     */
    String String::repeated(StringView string, size_t count)
    {
        if(!count || string.is_empty())
            return empty();

        char* buffer;
        auto impl = StringImpl::create_uninitialized(count * string.length(), buffer);
        for(size_t i = 0; i < count; i++)
            __builtin_memcpy(buffer + i * string.length(), string.characters_without_null_termination(), string.length());
        return *impl;
    }

    /**
     * @param value 
     * @param base 
     * @param map 
     * @return String 
     */
    String String::bijective_base_from(size_t value, unsigned base, StringView map)
    {
        if(map.is_null())
            map = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"sv;

        VERIFY(base >= 2 && base <= map.length());

        Array<char, round_up_to_power_of_two(sizeof(size_t) * 8 + 1, 2)> buffer;
        size_t i = 0;

        do
        {
            buffer[i++] = map[value % base];
            value /= base;
        } while(value > 0);

        if(i > 1)
            --buffer[i - 1];

        for(size_t j = 0; j < i / 2; ++j)
            swap(buffer[j], buffer[i - j - 1]);

        return String{ReadonlyBytes(buffer.data(), i)};
    }

    /**
     * @param value 
     * @return String 
     */
    String String::roman_number_from(size_t value)
    {
        if(value > 3999)
            return String::number(value);

        StringBuilder builder;

        while(value > 0)
        {
            if(value >= 1000)
            {
                builder.append('M');
                value -= 1000;
            }
            else if(value >= 900)
            {
                builder.append("CM"sv);
                value -= 900;
            }
            else if(value >= 500)
            {
                builder.append('D');
                value -= 500;
            }
            else if(value >= 400)
            {
                builder.append("CD"sv);
                value -= 400;
            }
            else if(value >= 100)
            {
                builder.append('C');
                value -= 100;
            }
            else if(value >= 90)
            {
                builder.append("XC"sv);
                value -= 90;
            }
            else if(value >= 50)
            {
                builder.append('L');
                value -= 50;
            }
            else if(value >= 40)
            {
                builder.append("XL"sv);
                value -= 40;
            }
            else if(value >= 10)
            {
                builder.append('X');
                value -= 10;
            }
            else if(value == 9)
            {
                builder.append("IX"sv);
                value -= 9;
            }
            else if(value >= 5 && value <= 8)
            {
                builder.append('V');
                value -= 5;
            }
            else if(value == 4)
            {
                builder.append("IV"sv);
                value -= 4;
            }
            else if(value <= 3)
            {
                builder.append('I');
                value -= 1;
            }
        }

        return builder.to_string();
    }

    /**
     * @param mask 
     * @param mask_spans 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::matches(StringView mask, Vector<MaskSpan>& mask_spans, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::matches(*this, mask, case_sensitivity, &mask_spans);
    }

    /**
     * @param mask 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::matches(StringView mask, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::matches(*this, mask, case_sensitivity);
    }

    /**
     * @param needle 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::contains(StringView needle, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::contains(*this, needle, case_sensitivity);
    }

    /**
     * @param needle 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool String::contains(char needle, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::contains(*this, StringView(&needle, 1), case_sensitivity);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::equals_ignoring_case(StringView other) const
    {
        return StringUtils::equals_ignoring_case(view(), other);
    }

    /**
     * @return String 
     */
    String String::reverse() const
    {
        StringBuilder reversed_string(length());
        for(size_t i = length(); i-- > 0;)
        {
            reversed_string.append(characters()[i]);
        }
        return reversed_string.to_string();
    }

    /**
     * @param html 
     * @return String 
     */
    String escape_html_entities(StringView html)
    {
        StringBuilder builder;
        for(size_t i = 0; i < html.length(); ++i)
        {
            if(html[i] == '<')
                builder.append("&lt;");
            else if(html[i] == '>')
                builder.append("&gt;");
            else if(html[i] == '&')
                builder.append("&amp;");
            else if(html[i] == '"')
                builder.append("&quot;");
            else
                builder.append(html[i]);
        }
        return builder.to_string();
    }

    /**
     * @brief Construct a new String::String object
     * 
     * @param string 
     */
    String::String(FlyString const& string)
        : m_impl(string.impl())
    {
    }

    /**
     * @return String 
     */
    String String::to_lowercase() const
    {
        if(!m_impl)
            return {};
        return m_impl->to_lowercase();
    }

    /**
     * @return String 
     */
    String String::to_uppercase() const
    {
        if(!m_impl)
            return {};
        return m_impl->to_uppercase();
    }

    /**
     * @return String 
     */
    String String::to_snakecase() const
    {
        return StringUtils::to_snakecase(*this);
    }

    /**
     * @return String 
     */
    String String::to_titlecase() const
    {
        return StringUtils::to_titlecase(*this);
    }

    /**
     * @param characters 
     * @param string 
     * @return true 
     * @return false 
     */
    bool operator<(char const* characters, String const& string)
    {
        return string.view() > characters;
    }

    /**
     * @param characters 
     * @param string 
     * @return true 
     * @return false 
     */
    bool operator>=(char const* characters, String const& string)
    {
        return string.view() <= characters;
    }

    /**
     * @param characters 
     * @param string 
     * @return true 
     * @return false 
     */
    bool operator>(char const* characters, String const& string)
    {
        return string.view() < characters;
    }

    /**
     * @param characters 
     * @param string 
     * @return true 
     * @return false 
     */
    bool operator<=(char const* characters, String const& string)
    {
        return string.view() >= characters;
    }

    /**
     * @param cstring 
     * @return true 
     * @return false 
     */
    bool String::operator==(char const* cstring) const
    {
        return view() == cstring;
    }

    /**
     * @param stream 
     * @param string 
     * @return InputStream& 
     */
    InputStream& operator>>(InputStream& stream, String& string)
    {
        StringBuilder builder;

        for(;;)
        {
            char next_char;
            stream >> next_char;

            if(stream.has_any_error())
            {
                stream.set_fatal_error();
                string = nullptr;
                return stream;
            }

            if(next_char)
            {
                builder.append(next_char);
            }
            else
            {
                string = builder.to_string();
                return stream;
            }
        }
    }

    /**
     * @param fmtstr 
     * @param params 
     * @return String 
     */
    String String::vformatted(StringView fmtstr, TypeErasedFormatParams& params)
    {
        StringBuilder builder;
        MUST(vformat(builder, fmtstr, params));
        return builder.to_string();
    }

    /**
     * @param needle 
     * @return Vector<size_t> 
     */
    Vector<size_t> String::find_all(StringView needle) const
    {
        return StringUtils::find_all(*this, needle);
    }
} // namespace Mods