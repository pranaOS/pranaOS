/**
 * @file utf8view.cpp
 * @author Krisna Pranav
 * @brief UTF8VIEW source
 * @version 1.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "assertions.h"
#include "logstream.h"
#include "utf8view.h"

namespace Mods
{
    /**
     * @param string 
     */
    Utf8View::Utf8View(const String& string)
        : m_string(string)
    {}

    /**
     * @param string 
     */
    Utf8View::Utf8View(const StringView& string)
        : m_string(string)
    {}
    
    /**
     * @param string 
     */
    Utf8View::Utf8View(const char* string)
        : m_string(string)
    {}

    /**
     * @return const unsigned* 
     */
    const unsigned char* Utf8View::begin_ptr() const
    {
        return (const unsigned char*)m_string.characters_without_null_termination();
    }

    /**
     * @return const unsigned* 
     */
    const unsigned char* Utf8View::end_ptr() const
    {
        return begin_ptr() + m_string.length();
    }

    /**
     * @return Utf8CodepointIterator 
     */
    Utf8CodepointIterator Utf8View::begin() const
    {
        return { begin_ptr(), (int)m_string.length() };
    }
    
    /**
     * @return Utf8CodepointIterator 
     */
    Utf8CodepointIterator Utf8View::end() const
    {
        return { end_ptr(), 0 };
    }

    /**
     * @param it 
     * @return int 
     */
    int Utf8View::byte_offset_of(const Utf8CodepointIterator& it) const
    {
        ASSERT(it.m_ptr >= begin_ptr());
        ASSERT(it.m_ptr <= end_ptr());

        return it.m_ptr - begin_ptr();
    }

    /**
     * @param byte_offset 
     * @param byte_length 
     * @return Utf8View 
     */
    Utf8View Utf8View::substring_view(int byte_offset, int byte_length)
    {
        StringView string = m_string.substring_view(byte_offset, byte_length);
        return Utf8View { string };
    }

    /**
     * @param byte 
     * @param out_code_point_length_in_bytes 
     * @param out_value 
     * @return true 
     * @return false 
     */
    static inline bool decode_first(
        unsigned char byte,
        int& out_code_point_length_in_bytes,
        u32& out_value
    )
    {
        if ((byte & 128) == 0) {
            out_value = byte;
            out_code_point_length_in_bytes = 1;
            return true;
        }

        return false;
    }
}