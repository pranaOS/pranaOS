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

    /**
     * @tparam UtfViewType 
     * @param view 
     * @return Vector<u16, 1> 
     */
    template<typename UtfViewType>
    static Vector<u16, 1> to_utf16_impl(UtfViewType const& view) 
    {
        Vector<u16, 1> utf16_data;
        utf16_data.ensure_capacity(view.length());

        for (auto code_point : view)
            code_point_to_utf16(utf16_data, code_point);
        
        return utf16_data;
    }

    /**
     * @param utf8_view 
     * @return Vector<u16, 1> 
     */
    Vector<u16, 1> utf8_to_utf16(StringView utf8_view)
    {
        return to_utf16_impl(Utf8View { utf8_view });
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
     * @return size_t 
     */
    size_t Utf16View::length_in_code_points() const
    {
        if (!m_length_in_code_points.has_value())
            m_length_in_code_points = calculate_length_in_code_points();
        
        return *m_length_in_code_points;
    }
} // namespace Mods