/**
 * @file line.h
 * @author Krisna Pranav
 * @brief line
 * @version 6.0
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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
        /// @brief Construct a new Attribute object
        Attribute() 
        { 
            reset(); 
        }

        static const u32 default_foreground_color = xterm_colors[7];
        static const u32 default_background_color = xterm_colors[0];

        void reset()
        {
            foreground_color = default_foreground_color;
            background_color = default_background_color;
            flags = Flags::NoAttributes;
        }

        u32 foreground_color;
        u32 background_color;

        String href;
        String href_id;

        enum Flags : u8 
        {
            NoAttributes = 0x00,
            Bold = 0x01,
            Italic = 0x02,
            Underline = 0x04,
            Negative = 0x08,
            Blink = 0x10,
            Touched = 0x20,
        };

        /**
         * @return true 
         * @return false 
         */
        bool is_untouched() const 
        { 
            return !(flags & Touched); 
        }

        u8 flags = Flags::NoAttributes;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Attribute& other) const
        {
            return foreground_color == other.foreground_color && background_color == other.background_color && flags == other.flags;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Attribute& other) const
        {
            return !(*this == other);
        }
    }; // struct Attribute

    class Line 
    {
        MOD_MAKE_NONCOPYABLE(Line);
        MOD_MAKE_NONMOVABLE(Line);

    public:

        /**
         * @param columns 
         */
        explicit Line(u16 columns);

        /// @brief Destroy the Line object
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
            if (!m_utf32 && code_point & 0xffffff80u)
                convert_to_utf32();

            if (m_utf32)
                m_code_points.as_u32[index] = code_point;
            else
                m_code_points.as_u8[index] = code_point;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_dirty() const 
        { 
            return m_dirty; 
        }

        /**
         * @param b 
         */
        void set_dirty(bool b) 
        { 
            m_dirty = b; 
        }

        /**
         * @return const Attribute* 
         */
        const Attribute* attributes() const 
        { 
            return m_attributes; 
        }

        /**
         * @return Attribute* 
         */
        Attribute* attributes() 
        { 
            return m_attributes; 
        }

        void convert_to_utf32();

        /**
         * @return true 
         * @return false 
         */
        bool is_utf32() const 
        { 
            return m_utf32; 
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
} // namespace VT