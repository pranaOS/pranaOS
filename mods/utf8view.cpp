/**
 * @file utf8view.cpp
 * @author Krisna Pranav
 * @brief UTF8VIEW source
 * @version 6.0
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
    Utf8View Utf8View::substring_view(int byte_offset, int byte_length) const
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
    static inline bool decode_first_byte(
        unsigned char byte,
        int& out_code_point_length_in_bytes,
        u32& out_value)
    {
        if ((byte & 128) == 0) {
            out_value = byte;
            out_code_point_length_in_bytes = 1;
            return true;
        }
        if ((byte & 64) == 0) {
            return false;
        }
        if ((byte & 32) == 0) {
            out_value = byte & 31;
            out_code_point_length_in_bytes = 2;
            return true;
        }
        if ((byte & 16) == 0) {
            out_value = byte & 15;
            out_code_point_length_in_bytes = 3;
            return true;
        }
        if ((byte & 8) == 0) {
            out_value = byte & 7;
            out_code_point_length_in_bytes = 4;
            return true;
        }

        return false;
    }

    /**
     * @param valid_bytes 
     * @return true 
     * @return false 
     */
    bool Utf8View::validate(size_t& valid_bytes) const
    {
        valid_bytes = 0;
        for (auto ptr = begin_ptr(); ptr < end_ptr(); ptr++) {
            int code_point_length_in_bytes;
            u32 value;
            bool first_byte_makes_sense = decode_first_byte(*ptr, code_point_length_in_bytes, value);
            if (!first_byte_makes_sense)
                return false;

            for (int i = 1; i < code_point_length_in_bytes; i++) {
                ptr++;
                if (ptr >= end_ptr())
                    return false;
                if (*ptr >> 6 != 2)
                    return false;
            }

            valid_bytes += code_point_length_in_bytes;
        }

        return true;
    }

    /**
     * @return size_t 
     */
    size_t Utf8View::calculate_length() const
    {
        size_t length = 0;
        for (auto code_point : *this) {
            (void)code_point;
            ++length;
        }
        return length;
    }

    /**
     * @param ptr 
     * @param length 
     */
    Utf8CodepointIterator::Utf8CodepointIterator(const unsigned char* ptr, int length)
        : m_ptr(ptr)
        , m_length(length)
    {}

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Utf8CodepointIterator::operator==(const Utf8CodepointIterator& other) const
    {
        return m_ptr == other.m_ptr && m_length == other.m_length;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Utf8CodepointIterator::operator!=(const Utf8CodepointIterator& other) const
    {
        return !(*this == other);
    }

    /**
     * @return Utf8CodepointIterator& 
     */
    Utf8CodepointIterator& Utf8CodepointIterator::operator++()
    {
        ASSERT(m_length > 0);

        int code_point_length_in_bytes = 0;
        u32 value;
        bool first_byte_makes_sense = decode_first_byte(*m_ptr, code_point_length_in_bytes, value);

        ASSERT(first_byte_makes_sense);
        (void)value;

        ASSERT(code_point_length_in_bytes <= m_length);
        m_ptr += code_point_length_in_bytes;
        m_length -= code_point_length_in_bytes;

        return *this;
    }

    /**
     * @return int 
     */
    int Utf8CodepointIterator::code_point_length_in_bytes() const
    {
        ASSERT(m_length > 0);
        int code_point_length_in_bytes = 0;
        u32 value;
        bool first_byte_makes_sense = decode_first_byte(*m_ptr, code_point_length_in_bytes, value);
        ASSERT(first_byte_makes_sense);
        return code_point_length_in_bytes;
    }

    /**
     * @return u32 
     */
    u32 Utf8CodepointIterator::operator*() const
    {
        ASSERT(m_length > 0);

        u32 code_point_value_so_far = 0;
        int code_point_length_in_bytes = 0;

        bool first_byte_makes_sense = decode_first_byte(m_ptr[0], code_point_length_in_bytes, code_point_value_so_far);
        if (!first_byte_makes_sense)
            dbgln("First byte doesn't look like working here the bytes: {}", StringView { (const char*)m_ptr, (size_t)m_length });
        ASSERT(first_byte_makes_sense);
        if (code_point_length_in_bytes > m_length)
            dbgln("Needed enough bytes (need {}, have {}), first byte is: {:#02x}, '{}'", code_point_length_in_bytes, m_length, m_ptr[0], (const char*)m_ptr);
        ASSERT(code_point_length_in_bytes <= m_length);

        for (int offset = 1; offset < code_point_length_in_bytes; offset++) {
            ASSERT(m_ptr[offset] >> 6 == 2);
            code_point_value_so_far <<= 6;
            code_point_value_so_far |= m_ptr[offset] & 63;
        }

        return code_point_value_so_far;
    }

}
