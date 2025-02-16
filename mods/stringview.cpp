/**
 * @file stringview.cpp
 * @author Krisna Pranav
 * @brief String View
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/any.h>
#include <mods/bytebuffer.h>
#include <mods/find.h>
#include <mods/function.h>
#include <mods/memory.h>
#include <mods/stringview.h>
#include <mods/vector.h>

#ifndef KERNEL
#   include <mods/flystring.h>
#   include <mods/string.h>
#endif

namespace Mods
{
    #ifndef KERNEL
    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param string 
     */
    StringView::StringView(String const& string)
        : m_characters(string.characters())
        , m_length(string.length())
    {}

    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param string 
     */
    StringView::StringView(FlyString const& string)
        : m_characters(string.characters())
        , m_length(string.length())
    {}
    #endif

    /**
     * @brief Construct a new StringView::StringView object
     * 
     * @param buffer 
     */
    StringView::StringView(ByteBuffer const& buffer)
        : m_characters((char const*)buffer.data())
        , m_length(buffer.size())
    {}

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::split_view(char const separator, bool keep_empty) const
    {
        StringView seperator_view { &seperator, 1 };
        return split_view(seperator_view, keep_empty);
    }

    /**
     * @param separator 
     * @param keep_empty 
     * @return Vector<StringView> 
     */
    Vector<StringView> StringView::split_view(StringView separator, bool keep_empty) const
    {
        Vector<StringView> parts;

        for_each_split_view(separator, keep_empty, [&](StringView view) {
            parts.append(view);
        });

        return parts;
    }
}