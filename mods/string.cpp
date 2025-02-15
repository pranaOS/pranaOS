/**
 * @file string.cpp
 * @author Krisna Pranav
 * @brief string
 * @version 6.0
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytebuffer.h>
#include <mods/flystring.h>
#include <mods/format.h>
#include <mods/function.h>
#include <mods/memory.h>
#include <mods/stdlibextra.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @param fly_string 
     * @return true 
     * @return false 
     */
    bool String::operator==(FlyString const& fly_string) const
    {
        return m_impl == fly_string.impl() || view() == fly_string.view();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator==(String const& other) const
    {
        return m_impl == other.impl() || view() == other.view();
    }

    /**

     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator==(StringView other) const
    {
        return view() == other;
    }
} // namespace Mods