/**
 * @file utf8view.h
 * @author Krisna Pranav
 * @brief UTF8VIEW
 * @version 1.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "types.h"
#include "string_view.h"

namespace Mods 
{

    class Utf8View;

    class Utf8CodepointIterator 
    {
        friend class Utf8View;

    public:
        /**
         * @brief Construct a new Utf 8 Codepoint Iterator object
         * 
         */
        Utf8CodepointIterator() { }
        ~Utf8CodepointIterator() { }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const Utf8CodepointIterator&) const;

        /**
         * @return true 
         * @return false 
         */
        bool operator!=(const Utf8CodepointIterator&) const;

        /**
         * @return Utf8CodepointIterator& 
         */
        Utf8CodepointIterator& operator++();

        /**
         * @return u32 
         */
        u32 operator*() const;

        /**
         * @param other 
         * @return ssize_t 
         */
        ssize_t operator-(const Utf8CodepointIterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

        /**
         * @return int 
         */
        int code_point_length_in_bytes() const;

        /**
         * @return true 
         * @return false 
         */
        bool done() const 
        { 
            return !m_length; 
        }

    private:
        /**
         * @param char 
         */
        Utf8CodepointIterator(const unsigned char*, int);
        const unsigned char* m_ptr { nullptr };
        int m_length { -1 };
    }; // Utf8CodepointIterator

    class Utf8View 
    {
    public:
        using Iterator = Utf8CodepointIterator;

        Utf8View() { }

        /**
         * @brief Construct a new Utf 8 View object
         * 
         */
        explicit Utf8View(const String&);
        explicit Utf8View(const StringView&);
        explicit Utf8View(const char*);
        ~Utf8View() { }

        /**
         * @return const StringView& 
         */
        const StringView& as_string() const 
        { 
            return m_string; 
        }

        /**
         * @return Utf8CodepointIterator 
         */
        Utf8CodepointIterator begin() const;
        Utf8CodepointIterator end() const;

        /**
         * @return const unsigned* 
         */
        const unsigned char* bytes() const 
        { 
            return begin_ptr(); 
        }

        /**
         * @return int 
         */
        int byte_length() const 
        { 
            return m_string.length(); 
        }

        /**
         * @return int 
         */
        int byte_offset_of(const Utf8CodepointIterator&) const;

        /**
         * @param byte_offset 
         * @param byte_length 
         * @return Utf8View 
         */
        Utf8View substring_view(int byte_offset, int byte_length) const;

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return m_string.is_empty(); 
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
            if (!m_have_length) {
                m_length = calculate_length();
                m_have_length = true;
            }
            return m_length;
        }

    private:

        /**
         * @return const unsigned* 
         */
        const unsigned char* begin_ptr() const;

        /**
         * @return const unsigned* 
         */
        const unsigned char* end_ptr() const;

        /**
         * @return size_t 
         */
        size_t calculate_length() const;

        StringView m_string;
        mutable size_t m_length { 0 };
        mutable bool m_have_length { false };
    }; // class Utf8View

}

using Mods::Utf8CodepointIterator;
using Mods::Utf8View;
