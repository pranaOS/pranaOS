/**
 * @file line.h
 * @author Krisna Pranav
 * @brief line
 * @version 6.0
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/noncopyable.h>
#include <mods/string.h>
#include <libvt/xtermcolors.h>

namespace VT
{
    struct Attribute
    {

    };

    class Line
    {
        MOD_MAKE_NONCOPYABLE(Line);
        MOD_MAKE_NONMOVABLE(Line);

    public:
        explicit Line(u16 columns);
        ~Line();

        void clear(Attribute);

        bool has_only_one_background_color() const;

        void set_length(u16);

        /**
         * @return u16 
         */
        u16 length() const
        {
            return m_length;
        }

        /**
         * @param index 
         * @return u32 
         */
        u32 code_point(size_t index) const
        {
            if (m_utf32)
                return m_code_points.as_u32[index];
            
            return m_code_points.as_u8[index];
        }

        /**
         * @param index 
         * @param code_point 
         */
        void set_code_point(size_t index, u32 code_point)
        {
            if (!m_utf32 && code_point)
                convert_to_utf32();

            if (m_utf32)
                m_code_points.as_u32[index] = code_point;
            else 
                m_code_points.as_u8[index] = code_point;
        }

    private:
        union
        {
            u8* as_u8;
            u32* as_u32;
        } m_code_points { nullptr };

        Attribute* m_attributes { nullptr };

        bool m_dirty { false };
        bool m_utf32 { false };
        u16 m_length { 0 };
    }; // class Line
} // namesapce VT 