/**
 * @file textalignment.h
 * @author Krisna Pranav
 * @brief text alignment
 * @version 6.0
 * @date 2024-08-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/string_view.h>

namespace Gfx 
{

    #define GFX_ENUMERATE_TEXT_ALIGNMENTS(M) \
        M(Center)                            \
        M(CenterLeft)                        \
        M(CenterRight)                       \
        M(TopLeft)                           \
        M(TopRight)                          \
        M(BottomLeft)                        \
        M(BottomRight)

    enum class TextAlignment 
    {
    #define __ENUMERATE(x) x,
        GFX_ENUMERATE_TEXT_ALIGNMENTS(__ENUMERATE)
    #undef __ENUMERATE
    };

    /**
     * @param alignment 
     * @return true 
     * @return false 
     */
    inline bool is_right_text_alignment(TextAlignment alignment)
    {
        switch (alignment) {
        case TextAlignment::CenterRight:
        case TextAlignment::TopRight:
        case TextAlignment::BottomRight:
            return true;
        default:
            return false;
        }
    }

    /**
     * @param alignment 
     * @return true 
     * @return false 
     */
    inline bool is_vertically_centered_text_alignment(TextAlignment alignment)
    {
        switch (alignment) {
        case TextAlignment::CenterLeft:
        case TextAlignment::CenterRight:
        case TextAlignment::Center:
            return true;
        default:
            return false;
        }
    }

    /**
     * @param string 
     * @return Optional<TextAlignment> 
     */
    inline Optional<TextAlignment> text_alignment_from_string(StringView string)
    {
    #define __ENUMERATE(x) \
        if (string == #x)  \
            return TextAlignment::x;
        GFX_ENUMERATE_TEXT_ALIGNMENTS(__ENUMERATE)
    #undef __ENUMERATE
        return {};
    }
    
    /**
     * @param text_alignment 
     * @return const char* 
     */
    inline const char* to_string(TextAlignment text_alignment)
    {
    #define __ENUMERATE(x)                      \
        if (text_alignment == TextAlignment::x) \
            return #x;
        GFX_ENUMERATE_TEXT_ALIGNMENTS(__ENUMERATE)
    #undef __ENUMERATE
        return {};
    }

} // namespace Gfx