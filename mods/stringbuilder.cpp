/**
 * @file stringbuilder.cpp
 * @author Krisna Pranav
 * @brief string builder
 * @version 6.0
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytebuffer.h>
#include <mods/checked.h>
#include <mods/printfimplementation.h>
#include <mods/stdlibextra.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>

namespace Mods
{
    /**
     * @param size 
     * @return ErrorOr<void> 
     */
    inline ErrorOr<void> StringBuilder::will_append(size_t size)
    {
        Checked<size_t> needed_capacity = m_buffer.size();
        needed_capacity += size;
        VERIFY(!needed_capacity.has_overflow());

        if (needed_capacity <= m_buffer.capacity())
            return {};
        
        Checked<size_t> expanded_capacity = needed_capacity;
        expanded_capacity *= 2;
        VERIFY(!expanded_capacity.has_overflow());
        TRY(m_buffer.try_ensure_capacity(expanded_capacity.value()));
        return {};
    }

    /**
     * @param initial_capacity 
     */
    StringBuilder::StringBuilder(size_t initial_capacity)
    {
        m_buffer.ensure_capacity(initial_capacity);
    }

    /**
     * @param string 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append(StringView string)
    {
        if (string.is_empty())
            return {};
        
        TRY(will_append(string.length()));
        TRY(m_buffer.try_append(string.characters_without_null_termination()));
        return {};
    }

    /**
     * @param ch 
     */
    void StringBuilder::append_as_lowercase(char ch)
    {
        if (ch >= 'A' && ch <= 'Z')
            append(ch + 0x20);
        else
            append(ch);
    }

    /**
     * @param string 
     */
    void StringBuilder::append_escaped_for_json(StringView string)
    {
        MUST(try_append_escaped_for_json(string));
    }

    /**
     * @param string 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> StringBuilder::try_append_escaped_for_json(StringView string)
    {
        for (auto ch : string) {
            switch (ch) {

            }
        }
    }

} // namespace Mods