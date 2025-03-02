/**
 * @file utf8view.h
 * @author Krisna Pranav
 * @brief UTF8VIEW
 * @version 6.0
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/types.h>

namespace Mods
{
    class Utf8View;

    class Utf8CodePointIterator
    {
        friend class Utf8View;

    public:
        /**
         * @brief Construct a new Utf8CodePointIterator object
         * 
         */
        Utf8CodePointIterator() = default;

        /**
         * @brief Destroy the Utf8CodePointIterator object
         * 
         */
        ~Utf8CodePointIterator() = default;

        /**
         * @return true 
         * @return false 
         */
        bool operator==(Utf8CodePointIterator const&) const = default;
        bool operator!=(Utf8CodePointIterator const&) const = default;

        /**
         * @return Utf8CodePointIterator& 
         */
        Utf8CodePointIterator& operator++();

        /**
         * @return u32 
         */
        u32 operator*() const;
        
        /**
         * @param offset 
         * @return Optional<u32> 
         */
        Optional<u32> peek(size_t offset = 0) const;

        /**
         * @param other 
         * @return ssize_t 
         */
        ssize_t operator-(Utf8CodePointIterator const& other) const
        {
            return m_ptr - other.m_ptr;
        }

        /**
         * @return size_t 
         */
        size_t underlying_code_point_length_in_bytes() const;

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes underlying_code_point_bytes() const;

        /**
         * @return true 
         * @return false 
         */
        bool done() const
        {
            return m_length == 0;
        }

    private:
        /**
         * @brief Construct a new Utf8CodePointIterator object
         * 
         * @param ptr 
         * @param length 
         */
        Utf8CodePointIterator(u8 const* ptr, size_t length)
            : m_ptr(ptr), m_length(length)
        {
        }

        u8 const* m_ptr{nullptr};
        size_t m_length{0};
    }; // class Utf8CodePointIterator
 
    class Utf8View
    {
    public:
        using Iterator = Utf8CodePointIterator;

        /**
         * @brief Construct a new Utf8View object
         * 
         */
        Utf8View() = default;

        /**
         * @brief Construct a new Utf8View object
         * 
         * @param string 
         */
        explicit Utf8View(String& string)
            : m_string(string.view())
        {
        }

        explicit constexpr Utf8View(StringView string)
            : m_string(string)
        {
        }

        /**
         * @brief Destroy the Utf8View object
         * 
         */
        ~Utf8View() = default;

        /**
         * @brief Construct a new Utf8View object
         * 
         */
        explicit Utf8View(String&&) = delete;

        /**
         * @return StringView 
         */
        StringView as_string() const
        {
            return m_string;
        }

        /**
         * @return Utf8CodePointIterator 
         */
        Utf8CodePointIterator begin() const
        {
            return {begin_ptr(), m_string.length()};
        }

        /**
         * @return Utf8CodePointIterator 
         */
        Utf8CodePointIterator end() const
        {
            return {end_ptr(), 0};
        }

        /**
         * @return Utf8CodePointIterator 
         */
        Utf8CodePointIterator iterator_at_byte_offset(size_t) const;

        /**
         * @return unsigned char const* 
         */
        unsigned char const* bytes() const
        {
            return begin_ptr();
        }

        /**
         * @return size_t 
         */
        size_t byte_length() const
        {
            return m_string.length();
        }

        /**
         * @return size_t 
         */
        size_t byte_offset_of(Utf8CodePointIterator const&) const;
        size_t byte_offset_of(size_t code_point_offset) const;

        /**
         * @param byte_offset 
         * @param byte_length 
         * @return Utf8View 
         */
        Utf8View substring_view(size_t byte_offset, size_t byte_length) const
        {
            return Utf8View{m_string.substring_view(byte_offset, byte_length)};
        }

        /**
         * @param byte_offset 
         * @return Utf8View 
         */
        Utf8View substring_view(size_t byte_offset) const
        {
            return substring_view(byte_offset, byte_length() - byte_offset);
        }

        /**
         * @param code_point_offset 
         * @param code_point_length 
         * @return Utf8View 
         */
        Utf8View unicode_substring_view(size_t code_point_offset, size_t code_point_length) const;

        /**
         * @param code_point_offset 
         * @return Utf8View 
         */
        Utf8View unicode_substring_view(size_t code_point_offset) const
        {
            return unicode_substring_view(code_point_offset, length() - code_point_offset);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const
        {
            return m_string.is_empty();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const
        {
            return m_string.is_null();
        }

        /**
         * @return true 
         * @return false 
         */
        bool starts_with(Utf8View const&) const;
        
        /**
         * @return true 
         * @return false 
         */
        bool contains(u32) const;

        /**
         * @param characters 
         * @param mode 
         * @return Utf8View 
         */
        Utf8View trim(Utf8View const& characters, TrimMode mode = TrimMode::Both) const;

        /**
         * @param it 
         * @return size_t 
         */
        size_t iterator_offset(Utf8CodePointIterator const& it) const
        {
            return byte_offset_of(it);
        }

        /**
         * @param valid_bytes 
         * @return true 
         * @return false 
         */
        bool validate(size_t& valid_bytes) const;

        /**
         * @return true 
         * @return false 
         */
        bool validate() const
        {
            size_t valid_bytes;
            return validate(valid_bytes);
        }

        /**
         * @return size_t 
         */
        size_t length() const
        {
            if(!m_have_length)
            {
                m_length = calculate_length();
                m_have_length = true;
            }
            return m_length;
        }

    private:
        /**
         * @return u8 const* 
         */
        u8 const* begin_ptr() const
        {
            return (u8 const*)m_string.characters_without_null_termination();
        }
        u8 const* end_ptr() const
        {
            return begin_ptr() + m_string.length();
        }
        size_t calculate_length() const;

        StringView m_string;
        mutable size_t m_length{0};
        mutable bool m_have_length{false};
    }; // class Utf8CodePointIterator
} // namespace Mods

using Mods::Utf8CodePointIterator;
using Mods::Utf8View;