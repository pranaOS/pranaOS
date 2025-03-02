/**
 * @file utf16view.h
 * @author Krisna Pranav
 * @brief UTF16 View
 * @version 6.0
 * @date 2024-11-02
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Mods
{
    Vector<u16, 1> utf8_to_utf16(StringView);
    Vector<u16, 1> utf8_to_utf16(Utf8View const&);
    Vector<u16, 1> utf32_to_utf16(Utf32View const&);
    void code_point_to_utf16(Vector<u16, 1>&, u32);

    class Utf16View;

    class Utf16CodePointIterator
    {
        friend class Utf16View;

    public:
        /**
         * @brief Construct a new Utf16CodePointIterator object
         * 
         */
        Utf16CodePointIterator() = default;

        /**
         * @brief Destroy the Utf16CodePointIterator object
         * 
         */
        ~Utf16CodePointIterator() = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf16CodePointIterator const& other) const
        {
            return (m_ptr == other.m_ptr) && (m_remaining_code_units == other.m_remaining_code_units);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Utf16CodePointIterator const& other) const
        {
            return !(*this == other);
        }

        /**
         * @return Utf16CodePointIterator& 
         */
        Utf16CodePointIterator& operator++();

        /**
         * @return u32 
         */
        u32 operator*() const;

        /**
         * @return size_t 
         */
        size_t length_in_code_units() const;

    private:
        /**
         * @brief Construct a new Utf16CodePointIterator object
         * 
         * @param ptr 
         * @param length 
         */
        Utf16CodePointIterator(u16 const* ptr, size_t length)
            : m_ptr(ptr), m_remaining_code_units(length)
        {
        }

        u16 const* m_ptr{nullptr};
        size_t m_remaining_code_units{0};
    }; // class Utf16CodePointIterator

    class Utf16View
    {
    public:
        /**
         * @return true 
         * @return false 
         */
        static bool is_high_surrogate(u16);
        static bool is_low_surrogate(u16);

        /**
         * @param high_surrogate 
         * @param low_surrogate 
         * @return u32 
         */
        static u32 decode_surrogate_pair(u16 high_surrogate, u16 low_surrogate);

        /**
         * @brief Construct a new Utf16View object
         * 
         */
        Utf16View() = default;

        /**
         * @brief Destroy the Utf16View object
         * 
         */
        ~Utf16View() = default;

        /**
         * @brief Construct a new Utf16View object
         * 
         * @param code_units 
         */
        explicit Utf16View(Span<u16 const> code_units)
            : m_code_units(code_units)
        {
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf16View const& other) const
        {
            return m_code_units == other.m_code_units;
        }

        enum class AllowInvalidCodeUnits
        {
            Yes,
            No,
        }; // enum class AllowInvalidCodeUnits

        /**
         * @return String 
         */
        String to_utf8(AllowInvalidCodeUnits = AllowInvalidCodeUnits::No) const;

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const
        {
            return m_code_units.is_null();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const
        {
            return m_code_units.is_empty();
        }

        /**
         * @return size_t 
         */
        size_t length_in_code_units() const
        {
            return m_code_units.size();
        }

        /**
         * @return size_t 
         */
        size_t length_in_code_points() const;

        /**
         * @return Utf16CodePointIterator 
         */
        Utf16CodePointIterator begin() const
        {
            return {begin_ptr(), m_code_units.size()};
        }

        /**
         * @return Utf16CodePointIterator 
         */
        Utf16CodePointIterator end() const
        {
            return {end_ptr(), 0};
        }

        /**
         * @return u16 const* 
         */
        u16 const* data() const
        {
            return m_code_units.data();
        }

        /**
         * @param index 
         * @return u16 
         */
        u16 code_unit_at(size_t index) const;

        /**
         * @param index 
         * @return u32 
         */
        u32 code_point_at(size_t index) const;

        /**
         * @param code_unit_offset 
         * @return size_t 
         */
        size_t code_point_offset_of(size_t code_unit_offset) const;

        /**
         * @param code_point_offset 
         * @return size_t 
         */
        size_t code_unit_offset_of(size_t code_point_offset) const;

        /**
         * @return size_t 
         */
        size_t code_unit_offset_of(Utf16CodePointIterator const&) const;

        /**
         * @param code_unit_offset 
         * @param code_unit_length 
         * @return Utf16View 
         */
        Utf16View substring_view(size_t code_unit_offset, size_t code_unit_length) const;

        /**
         * @param code_unit_offset 
         * @return Utf16View 
         */
        Utf16View substring_view(size_t code_unit_offset) const
        {
            return substring_view(code_unit_offset, length_in_code_units() - code_unit_offset);
        }

        /**
         * @param code_point_offset 
         * @param code_point_length 
         * @return Utf16View 
         */
        Utf16View unicode_substring_view(size_t code_point_offset, size_t code_point_length) const;

        /**
         * @param code_point_offset 
         * @return Utf16View 
         */
        Utf16View unicode_substring_view(size_t code_point_offset) const
        {
            return unicode_substring_view(code_point_offset, length_in_code_points() - code_point_offset);
        }

        /**
         * @param valid_code_units 
         * @return true 
         * @return false 
         */
        bool validate(size_t& valid_code_units) const;

        /**
         * @return true 
         * @return false 
         */
        bool validate() const
        {
            size_t valid_code_units;
            return validate(valid_code_units);
        }

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(Utf16View const&) const;

    private:
        /**
         * @return u16 const* 
         */
        u16 const* begin_ptr() const
        {
            return m_code_units.data();
        }

        /**
         * @return u16 const* 
         */
        u16 const* end_ptr() const
        {
            return begin_ptr() + m_code_units.size();
        }

        /**
         * @return size_t 
         */
        size_t calculate_length_in_code_points() const;

        Span<u16 const> m_code_units;
        mutable Optional<size_t> m_length_in_code_points;
    }; // class Utf16View
} // namespace Mods

/**
 * @tparam  
 */
template <>
struct Mods::Formatter<Mods::Utf16View> : Formatter<FormatString>
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Mods::Utf16View const& value)
    {
        return builder.builder().try_append(value);
    }
}; // struct Mods::Formatter<Mods::Utf16View> : Formatter<FormatString>

using Mods::Utf16View;
