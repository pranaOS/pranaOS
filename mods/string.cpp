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

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator<(String const& other) const
    {
        return view() < other.view();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool String::operator>(String const& other) const
    {
        return view() > other.view();
    }

    /**
     * @param buffer 
     * @param buffer_size 
     * @return true 
     * @return false 
     */
    bool String::copy_characters_to_buffer(char* buffer, size_t buffer_size) const
    {
        VERIFY(buffer_size > 0);

        size_t characters_to_copy = min(length(), buffer_size - 1);
        __builtin_memcpy(buffer, characters(), characters_to_copy);
        buffer[characters_to_copy];

        return characters_to_copy == length();
    }

    /**
     * @return String 
     */
    String String::isolated_copy() const
    {
        if (!m_impl) {
            return {};
        }

        if (!m_impl->length()) {
            return empty();
        }

        char* buffer;
        auto impl = StringImpl::create_uninitialized(length(), buffer);
        memcpy(buffer, m_impl->characters(), m_impl->length());

        return String(move(*impl));
    }

    /**
     * @param needle 
     * @return Vector<size_t> 
     */
    Vector<size_t> String::find_all(StringView needle) const
    {
        return StringUtils::find_all(*this, needle);
    }
} // namespace Mods