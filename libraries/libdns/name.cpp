/**
 * @file name.cpp
 * @author Krisna Pranav
 * @brief name
 * @version 6.0
 * @date 2025-03-10
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */


#include "name.h"
#include <mods/random.h>
#include <mods/vector.h>
#include <ctype.h>

namespace DNS 
{

    /**
     * @brief Construct a new Name::Name object
     * 
     * @param name 
     */
    Name::Name(String const& name)
    {
        if (name.ends_with('.'))
            m_name = name.substring(0, name.length() - 1);
        else
            m_name = name;
    }

    /**
     * @param data 
     * @param offset 
     * @param max_offset 
     * @param recursion_level 
     * @return Name 
     */
    Name Name::parse(u8 const* data, size_t& offset, size_t max_offset, size_t recursion_level)
    {
        if (recursion_level > 4)
            return {};

        StringBuilder builder;
        while (true) {
            if (offset >= max_offset)
                return {};
            u8 b = data[offset++];
            if (b == '\0') {
                return builder.to_string();
            } else if ((b & 0xc0) == 0xc0) {
                if (offset >= max_offset)
                    return {};
                size_t dummy = (b & 0x3f) << 8 | data[offset++];
                auto rest_of_name = parse(data, dummy, max_offset, recursion_level + 1);
                builder.append(rest_of_name.as_string());
                return builder.to_string();
            } else {
                if (offset + b >= max_offset)
                    return {};
                builder.append((char const*)&data[offset], (size_t)b);
                builder.append('.');
                offset += b;
            }
        }
    }

    /**
     * @return size_t 
     */
    size_t Name::serialized_size() const
    {
        if (m_name.is_empty())
            return 1;
        return m_name.length() + 2;
    }

    void Name::randomize_case()
    {
        StringBuilder builder;
        for (char c : m_name) {
            if (isalpha(c)) {
                if (get_random_uniform(2))
                    c |= 0x20;
                else
                    c &= ~0x20;
            }
            builder.append(c);
        }
        m_name = builder.to_string();
    }

    /**
     * @param stream 
     * @param name 
     * @return OutputStream& 
     */
    OutputStream& operator<<(OutputStream& stream, Name const& name)
    {
        auto parts = name.as_string().split_view('.');
        for (auto& part : parts) {
            stream << (u8)part.length();
            stream << part.bytes();
        }
        stream << '\0';
        return stream;
    }

    /**
     * @param name 
     * @return unsigned 
     */
    unsigned Name::Traits::hash(Name const& name)
    {
        return CaseInsensitiveStringTraits::hash(name.as_string());
    }

    /**
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    bool Name::Traits::equals(Name const& a, Name const& b)
    {
        return CaseInsensitiveStringTraits::equals(a.as_string(), b.as_string());
    }

} // namespace DNS
