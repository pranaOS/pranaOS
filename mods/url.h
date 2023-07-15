/**
 * @file url.h
 * @author Krisna Pranav
 * @brief url
 * @version 1.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "string.h"
#include "string_view.h"

namespace Mods 
{

    class URL 
    {
    public:
        URL() { }

        URL(const StringView&);

        URL(const char* string)
            : URL(StringView(string))
        {
        }

        URL(const String& string)
            : URL(string.view())
        {
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_valid; 
        }

        /**
         * @return String 
         */
        String protocol() const 
        { 
            return m_protocol; 
        }

        /**
         * @return String 
         */
        String host() const 
        { 
            return m_host; 
        }

        /**
         * @return String 
         */
        String path() const 
        { 
            return m_path; 
        }

        /**
         * @return String 
         */
        String query() const 
        { 
            return m_query; 
        }

        /**
         * @return String 
         */
        String fragment() const 
        { 
            return m_fragment; 
        }

        /**
         * @return u16 
         */
        u16 port() const 
        { 
            return m_port; 
        }

        /**
         * @param protocol 
         */
        void set_protocol(const String& protocol);

        /**
         * @param host 
         */
        void set_host(const String& host);

        /**
         * @param port 
         */
        void set_port(const u16 port);

        /**
         * @param path 
         */
        void set_path(const String& path);

        /**
         * @param query 
         */
        void set_query(const String& query);

        /**
         * @param fragment 
         */
        void set_fragment(const String& fragment);

        String basename() const;
        String to_string() const;

        /**
         * @return URL 
         */
        URL complete_url(const String&) const;

        /**
         * @return true 
         * @return false 
         */
        bool data_payload_is_base64() const 
        { 
            return m_data_payload_is_base64; 
        }

        /**
         * @return const String& 
         */
        const String& data_mime_type() const 
        { 
            return m_data_mime_type; 
        }

        /**
         * @return const String& 
         */
        const String& data_payload() const 
        { 
            return m_data_payload; 
        }

        /**
         * @param url_or_path 
         * @return URL 
         */
        static URL create_with_url_or_path(const String& url_or_path);

        /**
         * @param path 
         * @return URL 
         */
        static URL create_with_file_protocol(const String& path);

        /**
         * @param mime_type 
         * @param payload 
         * @param is_base64 
         * @return URL 
         */
        static URL create_with_data(const StringView& mime_type, const StringView& payload, bool is_base64 = false);

        /**
         * @param protocol 
         * @return true 
         * @return false 
         */
        static bool protocol_requires_port(const String& protocol);

        /**
         * @param protocol 
         * @return u16 
         */
        static u16 default_port_for_protocol(const String& protocol);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const URL& other) const
        {
            if (this == &other)
                return true;
            return to_string() == other.to_string();
        }

    private:
        bool parse(const StringView&);
        bool compute_validity() const;

        bool m_valid { false };
        u16 m_port { 0 };
        bool m_data_payload_is_base64 { false };
        String m_protocol;
        String m_host;
        String m_path;
        String m_query;
        String m_fragment;
        String m_data_mime_type;
        String m_data_payload;
    };
    
    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const URL& value)
    {
        return stream << value.to_string();
    }

    /**
     * @tparam t
     */
    template<>
    struct Formatter<URL> : Formatter<StringView> {
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const URL& value)
        {
            Formatter<StringView>::format(params, builder, value.to_string());
        }
    };

    template<>
    struct Traits<URL> : public GenericTraits<URL> {
        static unsigned hash(const URL& url) { return url.to_string().hash(); }
    };

}
