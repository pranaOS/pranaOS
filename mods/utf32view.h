/**
 * @file utf32view.h
 * @author Krisna Pranav
 * @brief Utf32View
 * @version 1.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"
#include "assertions.h"
#include "check.h"

namespace Mods 
{

    class Utf32View;

    class Utf32CodepointIterator 
    {
        friend class Utf32View;

    public:
        Utf32CodepointIterator() { }
        ~Utf32CodepointIterator() { }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Utf32CodepointIterator& other) const
        {
            return m_ptr == other.m_ptr && m_length == other.m_length;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Utf32CodepointIterator& other) const
        {
            return !(*this == other);
        }

        /**
         * @return Utf32CodepointIterator& 
         */
        Utf32CodepointIterator& operator++()
        {
            ASSERT(m_length > 0);
            m_ptr++;
            m_length--;
            return *this;
        }

        /**
         * @param other 
         * @return ssize_t 
         */
        ssize_t operator-(const Utf32CodepointIterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

        /**
         * @return u32 
         */
        u32 operator*() const
        {
            ASSERT(m_length > 0);
            return *m_ptr;
        }

        /**
         * @return constexpr int 
         */
        constexpr int code_point_length_in_bytes() const 
        { 
            return sizeof(u32); 
        }

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
         * @param ptr 
         * @param length 
         */
        Utf32CodepointIterator(const u32* ptr, size_t length)
            : m_ptr(ptr)
            , m_length((ssize_t)length)
        {}

        const u32* m_ptr { nullptr };
        ssize_t m_length { -1 };
    }; // Utf32CodepointIterator

    class Utf32View 
    {
    public:
        using Iterator = Utf32CodepointIterator;

        Utf32View() { }

        /**
         * @param code_points 
         * @param length 
         */
        Utf32View(const u32* code_points, size_t length)
            : m_code_points(code_points)
            , m_length(length)
        {
            ASSERT(code_points || length == 0);
        }

        /**
         * @return Utf32CodepointIterator 
         */
        Utf32CodepointIterator begin() const
        {
            return { begin_ptr(), m_length };
        }

        /**
         * @return Utf32CodepointIterator 
         */
        Utf32CodepointIterator end() const
        {
            return { end_ptr(), 0 };
        }

        /**
         * @return const u32* 
         */
        const u32* code_points() const 
        { 
            return m_code_points; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return m_length == 0; 
        }

        /**
         * @return size_t 
         */
        size_t length() const 
        { 
            return m_length; 
        }

        /**
         * @param offset 
         * @param length 
         * @return Utf32View 
         */
        Utf32View substring_view(size_t offset, size_t length) const
        {
            if (length == 0)
                return {};
            ASSERT(offset < m_length);
            ASSERT(!Checked<size_t>::addition_would_overflow(offset, length));
            ASSERT((offset + length) <= m_length);
            return Utf32View(m_code_points + offset, length);
        }

    private:

        /**
         * @return const u32* 
         */
        const u32* begin_ptr() const
        {
            return m_code_points;
        }

        /**
         * @return const u32* 
         */
        const u32* end_ptr() const
        {
            return m_code_points + m_length;
        }

        const u32* m_code_points { nullptr };
        size_t m_length { 0 };
    }; // Utf32View

}

using Mods::Utf32View;
