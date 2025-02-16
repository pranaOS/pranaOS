/**
 * @file stringview.cpp
 * @author Krisna Pranav
 * @brief String View
 * @version 6.0
 * @date 2025-02-16
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/any.h>
#include <mods/bytebuffer.h>
#include <mods/find.h>
#include <mods/function.h>
#include <mods/memory.h>
#include <mods/stringview.h>
#include <mods/vector.h>

#ifndef KERNEL
#include <mods/flystring.h>
#include <mods/string.h>
#endif

namespace Mods
{
    #ifndef KERNEL
    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param string 
     */
    StringView::StringView(String const& string)
        : m_characters(string.characters()), m_length(string.length())
    {
    }

    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param string 
     */
    StringView::StringView(FlyString const& string)
        : m_characters(string.characters()), m_length(string.length())
    {
    }
    #endif

    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param buffer 
     */
    StringView::StringView(ByteBuffer const& buffer)
        : m_characters((char const*)buffer.data()), m_length(buffer.size())
    {
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::split_view(char const separator, bool keep_empty) const
    {
        StringView seperator_view{&separator, 1};
        return split_view(seperator_view, keep_empty);
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::split_view(StringView separator, bool keep_empty) const
    {
        Vector<StringView> parts;
        for_each_split_view(separator, keep_empty, [&](StringView view)
                            { parts.append(view); });
        return parts;
    }

    /**
     * @param consider_cr 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::lines(bool consider_cr) const
    {
        if(is_empty())
            return {};

        if(!consider_cr)
            return split_view('\n', true);

        Vector<StringView> v;
        size_t substart = 0;
        bool last_ch_was_cr = false;
        bool split_view = false;

        for(size_t i = 0; i < length(); ++i)
        {
            char ch = characters_without_null_termination()[i];
            if(ch == '\n')
            {
                split_view = true;
                if(last_ch_was_cr)
                {
                    substart = i + 1;
                    split_view = false;
                }
            }
            if(ch == '\r')
            {
                split_view = true;
                last_ch_was_cr = true;
            }
            else
            {
                last_ch_was_cr = false;
            }
            if(split_view)
            {
                size_t sublen = i - substart;
                v.append(substring_view(substart, sublen));
                substart = i + 1;
            }
            split_view = false;
        }
        size_t taillen = length() - substart;
        if(taillen != 0)
            v.append(substring_view(substart, taillen));
        return v;
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    bool StringView::starts_with(char ch) const
    {
        if(is_empty())
            return false;
        return ch == characters_without_null_termination()[0];
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool StringView::starts_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::starts_with(*this, str, case_sensitivity);
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    bool StringView::ends_with(char ch) const
    {
        if(is_empty())
            return false;
        return ch == characters_without_null_termination()[length() - 1];
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool StringView::ends_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::ends_with(*this, str, case_sensitivity);
    }

    /**
     * @param mask 
     * @param mask_spans 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool StringView::matches(StringView mask, Vector<MaskSpan>& mask_spans, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::matches(*this, mask, case_sensitivity, &mask_spans);
    }

    /**
     * @param mask 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool StringView::matches(StringView mask, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::matches(*this, mask, case_sensitivity);
    }

    /**
     * @param needle 
     * @return true 
     * @return false 
     */
    bool StringView::contains(char needle) const
    {
        for(char current : *this)
        {
            if(current == needle)
                return true;
        }
        return false;
    }

    /**
     * @param needle 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool StringView::contains(StringView needle, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::contains(*this, needle, case_sensitivity);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool StringView::equals_ignoring_case(StringView other) const
    {
        return StringUtils::equals_ignoring_case(*this, other);
    }

    #ifndef KERNEL
    String StringView::to_lowercase_string() const
    {
        return StringImpl::create_lowercased(characters_without_null_termination(), length());
    }
    
    /**
     * @return String 
     */
    String StringView::to_uppercase_string() const
    {
        return StringImpl::create_uppercased(characters_without_null_termination(), length());
    }

    /**
     * @return String 
     */
    String StringView::to_titlecase_string() const
    {
        return StringUtils::to_titlecase(*this);
    }
    #endif

    /**
     * @param substring 
     * @return StringView 
     */
    StringView StringView::substring_view_starting_from_substring(StringView substring) const
    {
        char const* remaining_characters = substring.characters_without_null_termination();
        VERIFY(remaining_characters >= m_characters);
        VERIFY(remaining_characters <= m_characters + m_length);
        size_t remaining_length = m_length - (remaining_characters - m_characters);
        return {remaining_characters, remaining_length};
    }

    /**
     * @param substring 
     * @return StringView 
     */
    StringView StringView::substring_view_starting_after_substring(StringView substring) const
    {
        char const* remaining_characters = substring.characters_without_null_termination() + substring.length();
        VERIFY(remaining_characters >= m_characters);
        VERIFY(remaining_characters <= m_characters + m_length);
        size_t remaining_length = m_length - (remaining_characters - m_characters);
        return {remaining_characters, remaining_length};
    }

    /**
     * @param buffer 
     * @param buffer_size 
     * @return true 
     * @return false 
     */
    bool StringView::copy_characters_to_buffer(char* buffer, size_t buffer_size) const
    {
        VERIFY(buffer_size > 0);

        size_t characters_to_copy = min(m_length, buffer_size - 1);
        __builtin_memcpy(buffer, m_characters, characters_to_copy);
        buffer[characters_to_copy] = 0;

        return characters_to_copy == m_length;
    }

    /**
     * @tparam T 
     * @return Optional<T> 
     */
    template <typename T>
    Optional<T> StringView::to_int() const
    {
        return StringUtils::convert_to_int<T>(*this);
    }

    template Optional<i8> StringView::to_int() const;
    template Optional<i16> StringView::to_int() const;
    template Optional<i32> StringView::to_int() const;
    template Optional<long> StringView::to_int() const;
    template Optional<long long> StringView::to_int() const;

    /**
     * @tparam T 
     * @return Optional<T> 
     */
    template <typename T>
    Optional<T> StringView::to_uint() const
    {
        return StringUtils::convert_to_uint<T>(*this);
    }

    template Optional<u8> StringView::to_uint() const;
    template Optional<u16> StringView::to_uint() const;
    template Optional<u32> StringView::to_uint() const;
    template Optional<unsigned long> StringView::to_uint() const;
    template Optional<unsigned long long> StringView::to_uint() const;
    template Optional<long> StringView::to_uint() const;
    template Optional<long long> StringView::to_uint() const;

    #ifndef KERNEL
    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool StringView::operator==(String const& string) const
    {
        return *this == string.view();
    }

    /**
     * @return String 
     */
    String StringView::to_string() const
    {
        return String{*this};
    }

    /**
     * @param needle 
     * @param replacement 
     * @param all_occurrences 
     * @return String 
     */
    String StringView::replace(StringView needle, StringView replacement, bool all_occurrences) const
    {
        return StringUtils::replace(*this, needle, replacement, all_occurrences);
    }
    #endif

    /**
     * @param needle 
     * @return Vector<size_t> 
     */
    Vector<size_t> StringView::find_all(StringView needle) const
    {
        return StringUtils::find_all(*this, needle);
    }

    /**
     * @param predicate 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::split_view_if(Function<bool(char)> const& predicate, bool keep_empty) const
    {
        if(is_empty())
            return {};

        Vector<StringView> v;
        size_t substart = 0;
        for(size_t i = 0; i < length(); ++i)
        {
            char ch = characters_without_null_termination()[i];
            if(predicate(ch))
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
} // namespace Mods