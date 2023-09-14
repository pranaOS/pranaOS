/**
 * @file line.cpp
 * @author Krisna Pranav
 * @brief line
 * @version 6.0
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libvt/line.h>
#include <string.h>

namespace VT 
{
    /**
     * @param length 
     */
    Line::Line(u16 length)
    {
        set_length(length);
    }

    /// @brief Destroy the Line::Line object
    Line::~Line()
    {
        if (m_utf32)
            delete[] m_code_points.as_u32;
        else
            delete[] m_code_points.as_u8;
        delete[] m_attributes;
    }

    /** 
     * @tparam CodepointType 
     * @param new_length 
     * @param old_code_points 
     * @param old_length 
     * @return CodepointType* 
     */
    template<typename CodepointType>
    static CodepointType* create_new_code_point_array(size_t new_length, const CodepointType* old_code_points, size_t old_length)
    {
        auto* new_code_points = new CodepointType[new_length];

        for (size_t i = 0; i < new_length; ++i)
            new_code_points[i] = ' ';

        if (old_code_points) {
            for (size_t i = 0; i < min(old_length, new_length); ++i) {
                new_code_points[i] = old_code_points[i];
            }
        }

        delete[] old_code_points;
        
        return new_code_points;
    }

    /**
     * @param new_length 
     */
    void Line::set_length(u16 new_length)
    {
        if (m_length == new_length)
            return;

        if (m_utf32)
            m_code_points.as_u32 = create_new_code_point_array<u32>(new_length, m_code_points.as_u32, m_length);
        else
            m_code_points.as_u8 = create_new_code_point_array<u8>(new_length, m_code_points.as_u8, m_length);

        auto* new_attributes = new Attribute[new_length];

        if (m_attributes) {
            for (size_t i = 0; i < min(m_length, new_length); ++i)
                new_attributes[i] = m_attributes[i];
        }

        delete[] m_attributes;

        m_attributes = new_attributes;
        m_length = new_length;
    }

    /**
     * @param attribute 
     */
    void Line::clear(Attribute attribute)
    {
        if (m_dirty) {
            for (u16 i = 0; i < m_length; ++i) {
                set_code_point(i, ' ');
                m_attributes[i] = attribute;
            }
            return;
        }
        
        for (unsigned i = 0; i < m_length; ++i) {
            if (code_point(i) != ' ')
                m_dirty = true;
            set_code_point(i, ' ');
        }

        for (unsigned i = 0; i < m_length; ++i) {
            if (m_attributes[i] != attribute)
                m_dirty = true;
            m_attributes[i] = attribute;
        }
    }
    
    /**
     * @return true 
     * @return false 
     */
    bool Line::has_only_one_background_color() const
    {
        if (!m_length)
            return true;

        auto color = m_attributes[0].background_color;

        for (size_t i = 1; i < m_length; ++i) {
            if (m_attributes[i].background_color != color)
                return false;
        }

        return true;
    }

    /// @brief: Line[convert_to_utf32]
    void Line::convert_to_utf32()
    {
        ASSERT(!m_utf32);

        auto* new_code_points = new u32[m_length];

        for (size_t i = 0; i < m_length; ++i) {
            new_code_points[i] = m_code_points.as_u8[i];
        }

        delete m_code_points.as_u8;

        m_code_points.as_u32 = new_code_points;

        m_utf32 = true;
    }

} // namespace VT