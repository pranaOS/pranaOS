/**
 * @file url.cpp
 * @author Krisna Pranav
 * @brief URL
 * @version 1.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "string_builder.h"
#include "url.h"
#include "urlparser.h"
#include "lexical_path.h"

namespace Mods
{
    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    static inline bool is_valid_protocol_character(char ch)
    {
        return ch >= 'a' && ch <= 'z';
    }
    
    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    static inline bool is_valid_hostname_character(char ch)
    {
        return ch && ch != '/' && ch != ':';
    }

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    static inline bool is_digit(char ch)
    {
        return ch >= '0' && ch <= '9';
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool URL::parse(const StringView& string)
    {
        if (string.is_null())
            return false;

        enum class State
        {
            InProtocol,
            InHostname,
            InPort,
            InPath,
            InQuery,
            InDataMimeType,
            InDataPayload,
        };

        Vector<char, 256> buffer;
        State state { State::InProtocol };

        size_t index = 0;

        auto peek = [&]
        {
            if (index >= string.length())
                return '\0';
            
            return string[index];
        };

        auto consume = [&] {
            if (index >= string.length())
                return '\0';
            return string[index++];
        }

        // while (index < string.length()) {
        //     switch (state) {
        //     }
        // }
    }   

    /**
     * @param string 
     */
    URL::URL(const StringView& string)
    {
        m_valid = parse(string);
    }

    /**
     * @return String 
     */
    String URL::to_string() const
    {
        StringBuilder builder;
        builder.append(m_protocol);

        if (m_protocol == "about") {
            builder.append(':');
            builder.append(m_path);
            return builder.to_string();
        }

        if (m_protocol == "data") {
            builder.append(':');
            builder.append(m_data_mime_type);

            if (m_data_payload_is_base64)
                builder.append(";base64");

            builder.append(',');
            return builder.to_string();
        }
    }

    /**
     * @return String 
     */
    String URL::basename() const
    {
        if (!m_valid)
            return  {};
        
        return LexicalPath(m_path).basename();
    }
}