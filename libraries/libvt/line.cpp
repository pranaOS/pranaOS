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
}