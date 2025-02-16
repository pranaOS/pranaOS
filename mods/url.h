/**
 * @file url.h
 * @author Krisna Pranav
 * @brief url
 * @version 6.0
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/vector.h>

namespace Mods
{
    class URL
    {
        friend class URLParser;

    public:
        enum class PercentEncodeSet
        {
            C0Control,
            Fragment,
            Query,
            SpecialQuery,
            Path,
            Userinfo,
            Component,
            ApplicationXWWWFormUrlencoded,
            EncodeURI
        }; // enum class PercentEncodeSet

        enum class ExcludeFragment
        {
            No,
            Yes
        }; // enum class ExcludeFragment

        /**
         * @brief Construct a new URL object
         * 
         */
        URL() = default;

        /**
         * @brief Construct a new URL object
         * 
         */
        URL(StringView);

        /**
         * @brief Construct a new URL object
         * 
         * @param string 
         */
        URL(char const* string)
            : URL(StringView(string))
        {
        }

        /**
         * @brief Construct a new URL object
         * 
         * @param string 
         */
        URL(String const& string)
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
         * @return String const& 
         */
        String const& scheme() const
        {
            return m_scheme;
        }

        /**
         * @return String const& 
         */
        String const& protocol() const
        {
            return m_scheme;
        }

        /**
         * @return String const& 
         */
        String const& username() const
        {
            return m_username;
        }

        /**
         * @return String const& 
         */
        String const& password() const
        {
            return m_password;
        }

        /**
         * @return String const& 
         */
        String const& host() const
        {
            return m_host;
        }

        /**
         * @return Vector<String> const& 
         */
        Vector<String> const& paths() const
        {
            return m_paths;
        }

        /**
         * @return String const& 
         */
        String const& query() const
        {
            return m_query;
        }

        /**
         * @return String const& 
         */
        String const& fragment() const
        {
            return m_fragment;
        }

        /**
         * @return Optional<u16> 
         */
        Optional<u16> port() const
        {
            return m_port;
        }

        /**
         * @return u16 
         */
        u16 port_or_default() const
        {
            return m_port.value_or(default_port_for_scheme(m_scheme));
        }

        /**
         * @return true 
         * @return false 
         */
        bool cannot_be_a_base_url() const
        {
            return m_cannot_be_a_base_url;
        }

        /**
         * @return true 
         * @return false 
         */
        bool cannot_have_a_username_or_password_or_port() const
        {
            return m_host.is_null() || m_host.is_empty() || m_cannot_be_a_base_url || m_scheme == "file"sv;
        }

        /**
         * @return true 
         * @return false 
         */
        bool includes_credentials() const
        {
            return !m_username.is_empty() || !m_password.is_empty();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_special() const
        {
            return is_special_scheme(m_scheme);
        }

        /**
         * @brief Set the scheme object
         * 
         */
        void set_scheme(String);

        /**
         * @brief Set the protocol object
         * 
         * @param protocol 
         */
        void set_protocol(String protocol)
        {
            set_scheme(move(protocol));
        }

        /**
         * @brief Set the username object
         * 
         */
        void set_username(String);

        /**
         * @brief Set the password object
         * 
         */
        void set_password(String);

        /**
         * @brief Set the host object
         * 
         */
        void set_host(String);

        /**
         * @brief Set the port object
         * 
         */
        void set_port(Optional<u16>);

        /**
         * @brief Set the paths object
         * 
         */
        void set_paths(Vector<String>);

        /**
         * @brief Set the query object
         * 
         */
        void set_query(String);

        /**
         * @brief Set the fragment object
         * 
         */
        void set_fragment(String);

        /**
         * @brief Set the cannot be a base url object
         * 
         * @param value 
         */
        void set_cannot_be_a_base_url(bool value)
        {
            m_cannot_be_a_base_url = value;
        }

        /**
         * @param path 
         */
        void append_path(String path)
        {
            m_paths.append(move(path));
        }

        /**
         * @return String 
         */
        String path() const;
        String basename() const;

        String serialize(ExcludeFragment = ExcludeFragment::No) const;
        String serialize_for_display() const;
        String to_string() const
        {
            return serialize();
        }

        String serialize_origin() const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool equals(URL const& other, ExcludeFragment = ExcludeFragment::No) const;

        /**
         * @return URL 
         */
        URL complete_url(String const&) const;

        /**
         * @return true 
         * @return false 
         */
        bool data_payload_is_base64() const
        {
            return m_data_payload_is_base64;
        }

        /**
         * @return String const& 
         */
        String const& data_mime_type() const
        {
            return m_data_mime_type;
        }

        /**
         * @return String const& 
         */
        String const& data_payload() const
        {
            return m_data_payload;
        }

        /**
         * @brief Create a with url or path object
         * 
         * @return URL 
         */
        static URL create_with_url_or_path(String const&);

        /**
         * @brief Create a with file scheme object
         * 
         * @param path 
         * @param fragment 
         * @param hostname 
         * @return URL 
         */
        static URL create_with_file_scheme(String const& path, String const& fragment = {}, String const& hostname = {});

        /**
         * @brief Create a with file protocol object
         * 
         * @param path 
         * @param fragment 
         * @return URL 
         */
        static URL create_with_file_protocol(String const& path, String const& fragment = {})
        {
            return create_with_file_scheme(path, fragment);
        }

        /**
         * @brief Create a with help scheme object
         * 
         * @param path 
         * @param fragment 
         * @param hostname 
         * @return URL 
         */
        static URL create_with_help_scheme(String const& path, String const& fragment = {}, String const& hostname = {});

        /**
         * @brief Create a with data object
         * 
         * @param mime_type 
         * @param payload 
         * @param is_base64 
         * @return URL 
         */
        static URL create_with_data(String mime_type, String payload, bool is_base64 = false)
        {
            return URL(move(mime_type), move(payload), is_base64);
        };

        /**
         * @return true 
         * @return false 
         */
        static bool scheme_requires_port(StringView);

        /**
         * @return u16 
         */
        static u16 default_port_for_scheme(StringView);

        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        static bool is_special_scheme(StringView);

        enum class SpaceAsPlus
        {
            No,
            Yes,
        }; // enum class SpaceAsPlus

        /**
         * @param input 
         * @param set 
         * @return String 
         */
        static String percent_encode(StringView input, PercentEncodeSet set = PercentEncodeSet::Userinfo, SpaceAsPlus = SpaceAsPlus::No);

        /**
         * @param input 
         * @return String 
         */
        static String percent_decode(StringView input);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(URL const& other) const
        {
            return equals(other, ExcludeFragment::No);
        }

        /**
         * @param code_point 
         * @return true 
         * @return false 
         */
        static bool code_point_is_in_percent_encode_set(u32 code_point, URL::PercentEncodeSet);

    private:
        /**
         * @brief Construct a new URL object
         * 
         * @param data_mime_type 
         * @param data_payload 
         * @param payload_is_base64 
         */
        URL(String&& data_mime_type, String&& data_payload, bool payload_is_base64)
            : m_valid(true), m_scheme("data"), m_data_payload_is_base64(payload_is_base64), m_data_mime_type(move(data_mime_type)), m_data_payload(move(data_payload))
        {
        }

        bool compute_validity() const;

        /**
         * @return String 
         */
        String serialize_data_url() const;

        /**
         * @param code_point 
         * @param set 
         */
        static void append_percent_encoded_if_necessary(StringBuilder&, u32 code_point, PercentEncodeSet set = PercentEncodeSet::Userinfo);

        /**
         * @param code_point 
         */
        static void append_percent_encoded(StringBuilder&, u32 code_point);

        bool m_valid{false};

        String m_scheme;
        String m_username;
        String m_password;
        String m_host;
        
        Optional<u16> m_port;
        String m_path;
        Vector<String> m_paths;
        String m_query;
        String m_fragment;

        bool m_cannot_be_a_base_url{false};

        bool m_data_payload_is_base64{false};
        String m_data_mime_type;
        String m_data_payload;
    }; // class URL

    template <>
    struct Formatter<URL> : Formatter<StringView>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, URL const& value)
        {
            return Formatter<StringView>::format(builder, value.serialize());
        }
    }; // struct Formatter<URL> : Formatter<StringView>

    template <>
    struct Traits<URL> : public GenericTraits<URL>
    {
        /**
         * @param url 
         * @return unsigned 
         */
        static unsigned hash(URL const& url)
        {
            return url.to_string().hash();
        }
    }; // struct Traits<URL> : public GenericTraits<URL>

} // namespace Mods
