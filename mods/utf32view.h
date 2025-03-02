/**
 * @file utf32view.h
 * @author Krisna Pranav
 * @brief Utf32View
 * @version 6.0
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/checked.h>
#include <mods/types.h>

namespace Mods
{
    class Utf32View;

    class Utf32CodePointIterator
    {
        friend class Utf32View;

    public:
        /**
         * @brief Construct a new Utf32CodePointIterator object
         * 
         */
        Utf32CodePointIterator() = default;
            
        /**
         * @brief Destroy the Utf32CodePointIterator object
         * 
         */
        ~Utf32CodePointIterator() = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf32CodePointIterator const& other) const
        {
            return m_ptr == other.m_ptr && m_length == other.m_length;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Utf32CodePointIterator const& other) const
        {
            return !(*this == other);
        }

        /**
         * @return Utf32CodePointIterator& 
         */
        Utf32CodePointIterator& operator++()
        {
            VERIFY(m_length > 0);
            m_ptr++;
            m_length--;
            return *this;
        }

        /**
         * @param other 
         * @return ssize_t 
         */
        ssize_t operator-(Utf32CodePointIterator const& other) const
        {
            return m_ptr - other.m_ptr;
        }

        /**
         * @return u32 
         */
        u32 operator*() const
        {
            VERIFY(m_length > 0);
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
         * @brief Construct a new Utf32CodePointIterator object
         * 
         * @param ptr 
         * @param length 
         */
        Utf32CodePointIterator(u32 const* ptr, size_t length)
            : m_ptr(ptr), m_length((ssize_t)length)
        {
        }

        u32 const* m_ptr{nullptr};
        ssize_t m_length{-1};
    }; // class Utf32CodePointIterator

    class Utf32View
    {
    public:
        using Iterator = Utf32CodePointIterator;

        /**
         * @brief Construct a new Utf32View object
         * 
         */
        Utf32View() = default;

        /**
         * @brief Construct a new Utf32View object
         * 
         * @param code_points 
         * @param length 
         */
        Utf32View(u32 const* code_points, size_t length)
            : m_code_points(code_points), m_length(length)
        {
            VERIFY(code_points || length == 0);
        }   

        /**
         * @return Utf32CodePointIterator 
         */
        Utf32CodePointIterator begin() const
        {
            return {begin_ptr(), m_length};
        }

        /**
         * @return Utf32CodePointIterator 
         */
        Utf32CodePointIterator end() const
        {
            return {end_ptr(), 0};
        }

        /**
         * @param index 
         * @return u32 
         */
        u32 at(size_t index) const
        {
            VERIFY(index < m_length);
            return m_code_points[index];
        }

        /**
         * @param index 
         * @return u32 
         */
        u32 operator[](size_t index) const
        {
            return at(index);
        }

        /**
         * @return u32 const* 
         */
        u32 const* code_points() const
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
        bool is_null() const
        {
            return !m_code_points;
        }

        /**
         * @return size_t 
         */
        size_t length() const
        {
            return m_length;
        }

        /**
         * @param it 
         * @return size_t 
         */
        size_t iterator_offset(Utf32CodePointIterator const& it) const
        {
            VERIFY(it.m_ptr >= m_code_points);
            VERIFY(it.m_ptr < m_code_points + m_length);
            return ((ptrdiff_t)it.m_ptr - (ptrdiff_t)m_code_points) / sizeof(u32);
        }

        /**
         * @param offset 
         * @param length 
         * @return Utf32View 
         */
        Utf32View substring_view(size_t offset, size_t length) const
        {
            VERIFY(offset <= m_length);
            VERIFY(!Checked<size_t>::addition_would_overflow(offset, length));
            VERIFY((offset + length) <= m_length);
            return Utf32View(m_code_points + offset, length);
        }

    private:
        /**
         * @return u32 const* 
         */
        u32 const* begin_ptr() const
        {
            return m_code_points;
        }
        u32 const* end_ptr() const
        {
            return m_code_points + m_length;
        }

        u32 const* m_code_points{nullptr};
        size_t m_length{0};
    }; // class Utf32View
} // namespace Mods

using Mods::Utf32View;
