/**
 * @file utf16view.cpp
 * @author Krisna Pranav
 * @brief utf 16 view
 * @version 6.0
 * @date 2025-02-16
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/charactertypes.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <mods/utf16view.h>
#include <mods/utf32view.h>
#include <mods/utf8view.h>

namespace Mods
{
    static constexpr u16 high_surrogate_min = 0xd800;
    static constexpr u16 high_surrogate_max = 0xdbff;
    static constexpr u16 low_surrogate_min = 0xdc00;
    static constexpr u16 low_surrogate_max = 0xdfff;
    static constexpr u32 replacement_code_point = 0xfffd;
    static constexpr u32 first_supplementary_plane_code_point = 0x10000;

    /**
     * @tparam UtfViewType 
     */
    template <typename UtfViewType>
    static Vector<u16, 1> to_utf16_impl(UtfViewType const& view)
        requires(IsSame<UtfViewType, Utf8View> || IsSame<UtfViewType, Utf32View>)
    {
        Vector<u16, 1> utf16_data;
        utf16_data.ensure_capacity(view.length());

        for(auto code_point : view)
            code_point_to_utf16(utf16_data, code_point);

        return utf16_data;
    }

    /**
     * @param utf8_view 
     * @return Vector<u16, 1> 
     */
    Vector<u16, 1> utf8_to_utf16(StringView utf8_view)
    {
        return to_utf16_impl(Utf8View{utf8_view});
    }

    /**
     * @param utf8_view 
     * @return Vector<u16, 1> 
     */
    Vector<u16, 1> utf8_to_utf16(Utf8View const& utf8_view)
    {
        return to_utf16_impl(utf8_view);
    }

    /**
     * @param utf32_view 
     * @return Vector<u16, 1> 
     */
    Vector<u16, 1> utf32_to_utf16(Utf32View const& utf32_view)
    {
        return to_utf16_impl(utf32_view);
    }

    /**
     * @param string 
     * @param code_point 
     */
    void code_point_to_utf16(Vector<u16, 1>& string, u32 code_point)
    {
        VERIFY(is_unicode(code_point));

        if(code_point < first_supplementary_plane_code_point)
        {
            string.append(static_cast<u16>(code_point));
        }
        else
        {
            code_point -= first_supplementary_plane_code_point;
            string.append(static_cast<u16>(high_surrogate_min | (code_point >> 10)));
            string.append(static_cast<u16>(low_surrogate_min | (code_point & 0x3ff)));
        }
    }

    /**
     * @param code_unit 
     * @return true 
     * @return false 
     */
    bool Utf16View::is_high_surrogate(u16 code_unit)
    {
        return (code_unit >= high_surrogate_min) && (code_unit <= high_surrogate_max);
    }

    /**
     * @param code_unit 
     * @return true 
     * @return false 
     */
    bool Utf16View::is_low_surrogate(u16 code_unit)
    {
        return (code_unit >= low_surrogate_min) && (code_unit <= low_surrogate_max);
    }

    /**
     * @param high_surrogate 
     * @param low_surrogate 
     * @return u32 
     */
    u32 Utf16View::decode_surrogate_pair(u16 high_surrogate, u16 low_surrogate)
    {
        VERIFY(is_high_surrogate(high_surrogate));
        VERIFY(is_low_surrogate(low_surrogate));

        return ((high_surrogate - high_surrogate_min) << 10) + (low_surrogate - low_surrogate_min) + first_supplementary_plane_code_point;
    }

    /**
     * @param allow_invalid_code_units 
     * @return String 
     */
    String Utf16View::to_utf8(AllowInvalidCodeUnits allow_invalid_code_units) const
    {
        StringBuilder builder;

        if(allow_invalid_code_units == AllowInvalidCodeUnits::Yes)
        {
            for(auto const* ptr = begin_ptr(); ptr < end_ptr(); ++ptr)
            {
                if(is_high_surrogate(*ptr))
                {
                    auto const* next = ptr + 1;

                    if((next < end_ptr()) && is_low_surrogate(*next))
                    {
                        auto code_point = decode_surrogate_pair(*ptr, *next);
                        builder.append_code_point(code_point);
                        ++ptr;
                        continue;
                    }
                }

                builder.append_code_point(static_cast<u32>(*ptr));
            }
        }
        else
        {
            for(auto code_point : *this)
                builder.append_code_point(code_point);
        }

        return builder.build();
    }

    /**
     * @return size_t 
     */
    size_t Utf16View::length_in_code_points() const
    {
        if(!m_length_in_code_points.has_value())
            m_length_in_code_points = calculate_length_in_code_points();
        return *m_length_in_code_points;
    }

    /**
     * @param index 
     * @return u16 
     */
    u16 Utf16View::code_unit_at(size_t index) const
    {
        VERIFY(index < length_in_code_units());
        return m_code_units[index];
    }

    /**
     * @param index 
     * @return u32 
     */
    u32 Utf16View::code_point_at(size_t index) const
    {
        VERIFY(index < length_in_code_units());

        u32 code_point = code_unit_at(index);
        if(!is_high_surrogate(code_point) && !is_low_surrogate(code_point))
            return code_point;
        if(is_low_surrogate(code_point) || (index + 1 == length_in_code_units()))
            return code_point;

        auto second = code_unit_at(index + 1);
        if(!is_low_surrogate(second))
            return code_point;

        return decode_surrogate_pair(code_point, second);
    }

    /**
     * @param code_unit_offset 
     * @return size_t 
     */
    size_t Utf16View::code_point_offset_of(size_t code_unit_offset) const
    {
        size_t code_point_offset = 0;

        for(auto it = begin(); it != end(); ++it)
        {
            if(code_unit_offset == 0)
                return code_point_offset;

            code_unit_offset -= it.length_in_code_units();
            ++code_point_offset;
        }

        return code_point_offset;
    }

    /**
     * @param code_point_offset 
     * @return size_t 
     */
    size_t Utf16View::code_unit_offset_of(size_t code_point_offset) const
    {
        size_t code_unit_offset = 0;

        for(auto it = begin(); it != end(); ++it)
        {
            if(code_point_offset == 0)
                return code_unit_offset;

            code_unit_offset += it.length_in_code_units();
            --code_point_offset;
        }

        return code_unit_offset;
    }

    /**
     * @param it 
     * @return size_t 
     */
    size_t Utf16View::code_unit_offset_of(Utf16CodePointIterator const& it) const
    {
        VERIFY(it.m_ptr >= begin_ptr());
        VERIFY(it.m_ptr <= end_ptr());

        return it.m_ptr - begin_ptr();
    }

    /**
     * @param code_unit_offset 
     * @param code_unit_length 
     * @return Utf16View 
     */
    Utf16View Utf16View::substring_view(size_t code_unit_offset, size_t code_unit_length) const
    {
        VERIFY(!Checked<size_t>::addition_would_overflow(code_unit_offset, code_unit_length));
        VERIFY(code_unit_offset + code_unit_length <= length_in_code_units());

        return Utf16View{m_code_units.slice(code_unit_offset, code_unit_length)};
    }

    /**
     * @param code_point_offset 
     * @param code_point_length 
     * @return Utf16View 
     */
    Utf16View Utf16View::unicode_substring_view(size_t code_point_offset, size_t code_point_length) const
    {
        if(code_point_length == 0)
            return {};

        auto code_unit_offset_of = [&](Utf16CodePointIterator const& it)
        { return it.m_ptr - begin_ptr(); };
        size_t code_point_index = 0;
        size_t code_unit_offset = 0;

        for(auto it = begin(); it != end(); ++it)
        {
            if(code_point_index == code_point_offset)
                code_unit_offset = code_unit_offset_of(it);

            if(code_point_index == (code_point_offset + code_point_length - 1))
            {
                size_t code_unit_length = code_unit_offset_of(++it) - code_unit_offset;
                return substring_view(code_unit_offset, code_unit_length);
            }

            ++code_point_index;
        }

        VERIFY_NOT_REACHED();
    }

    /**
     * @param valid_code_units 
     * @return true 
     * @return false 
     */
    bool Utf16View::validate(size_t& valid_code_units) const
    {
        valid_code_units = 0;

        for(auto const* ptr = begin_ptr(); ptr < end_ptr(); ++ptr)
        {
            if(is_high_surrogate(*ptr))
            {
                if((++ptr >= end_ptr()) || !is_low_surrogate(*ptr))
                    return false;
                ++valid_code_units;
            }
            else if(is_low_surrogate(*ptr))
            {
                return false;
            }

            ++valid_code_units;
        }

        return true;
    }
    
    /**
     * @return size_t 
     */
    size_t Utf16View::calculate_length_in_code_points() const
    {
        size_t code_points = 0;
        for([[maybe_unused]] auto code_point : *this)
            ++code_points;
        return code_points;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Utf16View::equals_ignoring_case(Utf16View const& other) const
    {
        if(length_in_code_units() == 0)
            return other.length_in_code_units() == 0;
        if(length_in_code_units() != other.length_in_code_units())
            return false;

        for(size_t i = 0; i < length_in_code_units(); ++i)
        {
            if(to_ascii_lowercase(m_code_units[i]) != to_ascii_lowercase(other.m_code_units[i]))
                return false;
        }

        return true;
    }

    /**
     * @return Utf16CodePointIterator& 
     */
    Utf16CodePointIterator& Utf16CodePointIterator::operator++()
    {
        size_t code_units = length_in_code_units();

        if(code_units > m_remaining_code_units)
        {
            m_ptr += m_remaining_code_units;
            m_remaining_code_units = 0;
        }
        else
        {
            m_ptr += code_units;
            m_remaining_code_units -= code_units;
        }

        return *this;
    }

    /**
     * @return u32 
     */
    u32 Utf16CodePointIterator::operator*() const
    {
        VERIFY(m_remaining_code_units > 0);

        if(Utf16View::is_high_surrogate(*m_ptr))
        {
            if((m_remaining_code_units > 1) && Utf16View::is_low_surrogate(*(m_ptr + 1)))
                return Utf16View::decode_surrogate_pair(*m_ptr, *(m_ptr + 1));
            return replacement_code_point;
        }
        else if(Utf16View::is_low_surrogate(*m_ptr))
        {
            return replacement_code_point;
        }

        return static_cast<u32>(*m_ptr);
    }

    /**
     * @return size_t 
     */
    size_t Utf16CodePointIterator::length_in_code_units() const
    {
        VERIFY(m_remaining_code_units > 0);

        if(Utf16View::is_high_surrogate(*m_ptr))
        {
            if((m_remaining_code_units > 1) && Utf16View::is_low_surrogate(*(m_ptr + 1)))
                return 2;
        }

        return 1;
    }

} // namespace Mods
