/**
 * @file url.cpp
 * @author Krisna Pranav
 * @brief URL
 * @version 6.0
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/charactertypes.h>
#include <mods/debug.h>
#include <mods/lexicalpath.h>
#include <mods/stringbuilder.h>
#include <mods/url.h>
#include <mods/urlparser.h>
#include <mods/utf8view.h>

namespace Mods
{

    /**
     * @brief Construct a new URL::URL object
     * 
     * @param string 
     */
    URL::URL(StringView string)
        : URL(URLParser::parse(string))
    {
        if constexpr(URL_PARSER_DEBUG)
        {
            if(m_valid)
                dbgln("URL constructor: Parsed URL to be '{}'.", serialize());
            else
                dbgln("URL constructor: Parsed URL to be invalid.");
        }
    }

    /**
     * @return String 
     */
    String URL::path() const
    {
        if(cannot_be_a_base_url())
            return paths()[0];
        StringBuilder builder;
        for(auto& path : m_paths)
        {
            builder.append('/');
            builder.append(path);
        }
        return builder.to_string();
    }
    
    /**
     * @param string 
     * @return URL 
     */
    URL URL::complete_url(String const& string) const
    {
        if(!is_valid())
            return {};

        return URLParser::parse(string, this);
    }

    /**
     * @param scheme 
     */
    void URL::set_scheme(String scheme)
    {
        m_scheme = move(scheme);
        m_valid = compute_validity();
    }

    /**
     * @param username 
     */
    void URL::set_username(String username)
    {
        m_username = move(username);
        m_valid = compute_validity();
    }

    /**
     * @param password 
     */
    void URL::set_password(String password)
    {
        m_password = move(password);
        m_valid = compute_validity();
    }

    /**
     * @param host 
     */
    void URL::set_host(String host)
    {
        m_host = move(host);
        m_valid = compute_validity();
    }   

    /**
     * @param port 
     */
    void URL::set_port(Optional<u16> port)
    {
        if(port == default_port_for_scheme(m_scheme))
        {
            m_port = {};
            return;
        }
        m_port = move(port);
        m_valid = compute_validity();
    }

    /**
     * @param paths 
     */
    void URL::set_paths(Vector<String> paths)
    {
        m_paths = move(paths);
        m_valid = compute_validity();
    }

    /**
     * @param query 
     */
    void URL::set_query(String query)
    {
        m_query = move(query);
    }

    /**
     * @param fragment 
     */
    void URL::set_fragment(String fragment)
    {
        m_fragment = move(fragment);
    }

    /**
     * @return true 
     * @return false 
     */
    bool URL::compute_validity() const
    {
        if(m_scheme.is_empty())
            return false;

        if(m_scheme == "data")
        {
            if(m_data_mime_type.is_empty())
                return false;
            if(m_data_payload_is_base64)
            {
                if(m_data_payload.length() % 4 != 0)
                    return false;
                for(auto character : m_data_payload)
                {
                    if(!is_ascii_alphanumeric(character) || character == '+' || character == '/' || character == '=')
                        return false;
                }
            }
        }
        else if(m_cannot_be_a_base_url)
        {
            if(m_paths.size() != 1)
                return false;
            if(m_paths[0].is_empty())
                return false;
        }
        else
        {
            if(m_scheme.is_one_of("about", "mailto"))
                return false;
            
            if(m_paths.size() == 0)
                return false;
        }

        if(m_scheme == "file" && m_host.is_null())
            return false;

        return true;
    }

    /**
     * @param scheme 
     * @return true 
     * @return false 
     */
    bool URL::scheme_requires_port(StringView scheme)
    {
        return (default_port_for_scheme(scheme) != 0);
    }

    /**
     * @param scheme 
     * @return u16 
     */
    u16 URL::default_port_for_scheme(StringView scheme)
    {
        if(scheme == "http")
            return 80;
        if(scheme == "https")
            return 443;
        if(scheme == "gemini")
            return 1965;
        if(scheme == "irc")
            return 6667;
        if(scheme == "ircs")
            return 6697;
        if(scheme == "ws")
            return 80;
        if(scheme == "wss")
            return 443;
        return 0;
    }

    /**
     * @param path 
     * @param fragment 
     * @param hostname 
     * @return URL 
     */
    URL URL::create_with_file_scheme(String const& path, String const& fragment, String const& hostname)
    {
        LexicalPath lexical_path(path);
        if(!lexical_path.is_absolute())
            return {};

        URL url;
        url.set_scheme("file");

        url.set_host(hostname.is_null() || hostname == "localhost" ? String::empty() : hostname);
        url.set_paths(lexical_path.parts());
       
        if(path.ends_with('/'))
            url.append_path("");
        url.set_fragment(fragment);
        return url;
    }

    /**
     * @param path 
     * @param fragment 
     * @param hostname 
     * @return URL 
     */
    URL URL::create_with_help_scheme(String const& path, String const& fragment, String const& hostname)
    {
        LexicalPath lexical_path(path);

        URL url;
        url.set_scheme("help");
        
        url.set_host(hostname.is_null() || hostname == "localhost" ? String::empty() : hostname);
        url.set_paths(lexical_path.parts());

        if(path.ends_with('/'))
            url.append_path("");
        url.set_fragment(fragment);
        return url;
    }

    /**
     * @param url_or_path 
     * @return URL 
     */
    URL URL::create_with_url_or_path(String const& url_or_path)
    {
        URL url = url_or_path;
        if(url.is_valid())
            return url;

        String path = LexicalPath::canonicalized_path(url_or_path);
        return URL::create_with_file_scheme(path);
    }

    /**
     * @param scheme 
     * @return true 
     * @return false 
     */
    bool URL::is_special_scheme(StringView scheme)
    {
        return scheme.is_one_of("ftp", "file", "http", "https", "ws", "wss");
    }

    /**
     * @return String 
     */
    String URL::serialize_data_url() const
    {
        VERIFY(m_scheme == "data");
        VERIFY(!m_data_mime_type.is_null());
        VERIFY(!m_data_payload.is_null());
        StringBuilder builder;
        builder.append(m_scheme);
        builder.append(':');
        builder.append(m_data_mime_type);
        if(m_data_payload_is_base64)
            builder.append(";base64");
        builder.append(',');
        
        builder.append(URL::percent_encode(m_data_payload, PercentEncodeSet::C0Control));
        return builder.to_string();
    }

    /**
     * @param exclude_fragment 
     * @return String 
     */
    String URL::serialize(ExcludeFragment exclude_fragment) const
    {
        if(m_scheme == "data")
            return serialize_data_url();
        StringBuilder builder;
        builder.append(m_scheme);
        builder.append(':');

        if(!m_host.is_null())
        {
            builder.append("//");

            if(includes_credentials())
            {
                builder.append(percent_encode(m_username, PercentEncodeSet::Userinfo));
                if(!m_password.is_empty())
                {
                    builder.append(':');
                    builder.append(percent_encode(m_password, PercentEncodeSet::Userinfo));
                }
                builder.append('@');
            }

            builder.append(m_host);
            if(m_port.has_value())
                builder.appendff(":{}", *m_port);
        }

        if(cannot_be_a_base_url())
        {
            builder.append(percent_encode(m_paths[0], PercentEncodeSet::Path));
        }
        else
        {
            if(m_host.is_null() && m_paths.size() > 1 && m_paths[0].is_empty())
                builder.append("/.");
            for(auto& segment : m_paths)
            {
                builder.append('/');
                builder.append(percent_encode(segment, PercentEncodeSet::Path));
            }
        }

        if(!m_query.is_null())
        {
            builder.append('?');
            builder.append(percent_encode(m_query, is_special() ? URL::PercentEncodeSet::SpecialQuery : URL::PercentEncodeSet::Query));
        }

        if(exclude_fragment == ExcludeFragment::No && !m_fragment.is_null())
        {
            builder.append('#');
            builder.append(percent_encode(m_fragment, PercentEncodeSet::Fragment));
        }

        return builder.to_string();
    }

    /**
     * @return String 
     */
    String URL::serialize_for_display() const
    {
        VERIFY(m_valid);
        if(m_scheme == "data")
            return serialize_data_url();
        StringBuilder builder;
        builder.append(m_scheme);
        builder.append(':');

        if(!m_host.is_null())
        {
            builder.append("//");
            builder.append(m_host);
            if(m_port.has_value())
                builder.appendff(":{}", *m_port);
        }

        if(cannot_be_a_base_url())
        {
            builder.append(percent_encode(m_paths[0], PercentEncodeSet::Path));
        }
        else
        {
            if(m_host.is_null() && m_paths.size() > 1 && m_paths[0].is_empty())
                builder.append("/.");
            for(auto& segment : m_paths)
            {
                builder.append('/');
                builder.append(percent_encode(segment, PercentEncodeSet::Path));
            }
        }

        if(!m_query.is_null())
        {
            builder.append('?');
            builder.append(percent_encode(m_query, is_special() ? URL::PercentEncodeSet::SpecialQuery : URL::PercentEncodeSet::Query));
        }

        if(!m_fragment.is_null())
        {
            builder.append('#');
            builder.append(percent_encode(m_fragment, PercentEncodeSet::Fragment));
        }

        return builder.to_string();
    }

    /**
     * @return String 
     */
    String URL::serialize_origin() const
    {
        VERIFY(m_valid);

        if(m_scheme == "blob"sv)
        {
            VERIFY(!m_paths.is_empty());
            URL url = m_paths[0];

            if(!url.is_valid())
                return "null";
            return url.serialize_origin();
        }
        else if(!m_scheme.is_one_of("ftp"sv, "http"sv, "https"sv, "ws"sv, "wss"sv))
        { 
            return "null";
        }

        StringBuilder builder;
        builder.append(m_scheme);
        builder.append("://"sv);
        builder.append(m_host);
        if(m_port.has_value())
            builder.append(":{}", *m_port);
        return builder.build();
    }

    /**
     * @param other 
     * @param exclude_fragments 
     * @return true 
     * @return false 
     */
    bool URL::equals(URL const& other, ExcludeFragment exclude_fragments) const
    {
        if(this == &other)
            return true;
        if(!m_valid || !other.m_valid)
            return false;
        return serialize(exclude_fragments) == other.serialize(exclude_fragments);
    }

    /**
     * @return String 
     */
    String URL::basename() const
    {
        if(!m_valid)
            return {};
        if(m_paths.is_empty())
            return {};
        return m_paths.last();
    }

    /**
     * @param builder 
     * @param code_point 
     */
    void URL::append_percent_encoded(StringBuilder& builder, u32 code_point)
    {
        if(code_point <= 0x7f)
            builder.appendff("%{:02X}", code_point);
        else if(code_point <= 0x07ff)
            builder.appendff("%{:02X}%{:02X}", ((code_point >> 6) & 0x1f) | 0xc0, (code_point & 0x3f) | 0x80);
        else if(code_point <= 0xffff)
            builder.appendff("%{:02X}%{:02X}%{:02X}", ((code_point >> 12) & 0x0f) | 0xe0, ((code_point >> 6) & 0x3f) | 0x80, (code_point & 0x3f) | 0x80);
        else if(code_point <= 0x10ffff)
            builder.appendff("%{:02X}%{:02X}%{:02X}%{:02X}", ((code_point >> 18) & 0x07) | 0xf0, ((code_point >> 12) & 0x3f) | 0x80, ((code_point >> 6) & 0x3f) | 0x80, (code_point & 0x3f) | 0x80);
        else
            VERIFY_NOT_REACHED();
    }

    /**
     * @param code_point 
     * @param set 
     * @return true 
     * @return false 
     */
    bool URL::code_point_is_in_percent_encode_set(u32 code_point, URL::PercentEncodeSet set)
    {
        switch(set)
        {
        case URL::PercentEncodeSet::C0Control:
            return code_point < 0x20 || code_point > 0x7E;
        case URL::PercentEncodeSet::Fragment:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::C0Control) || " \"<>`"sv.contains(code_point);
        case URL::PercentEncodeSet::Query:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::C0Control) || " \"#<>"sv.contains(code_point);
        case URL::PercentEncodeSet::SpecialQuery:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::Query) || code_point == '\'';
        case URL::PercentEncodeSet::Path:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::Query) || "?`{}"sv.contains(code_point);
        case URL::PercentEncodeSet::Userinfo:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::Path) || "/:;=@[\\]^|"sv.contains(code_point);
        case URL::PercentEncodeSet::Component:
            return code_point_is_in_percent_encode_set(code_point, URL::PercentEncodeSet::Userinfo) || "$%&+,"sv.contains(code_point);
        case URL::PercentEncodeSet::ApplicationXWWWFormUrlencoded:
            return code_point >= 0x7E || !(is_ascii_alphanumeric(code_point) || "!'()~"sv.contains(code_point));
        case URL::PercentEncodeSet::EncodeURI:
            return code_point >= 0x7E || (!is_ascii_alphanumeric(code_point) && !";,/?:@&=+$-_.!~*'()#"sv.contains(code_point));
        default:
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @param builder 
     * @param code_point 
     * @param set 
     */
    void URL::append_percent_encoded_if_necessary(StringBuilder& builder, u32 code_point, URL::PercentEncodeSet set)
    {
        if(code_point_is_in_percent_encode_set(code_point, set))
            append_percent_encoded(builder, code_point);
        else
            builder.append_code_point(code_point);
    }

    /**     
     * @param input 
     * @param set 
     * @param space_as_plus 
     * @return String 
     */
    String URL::percent_encode(StringView input, URL::PercentEncodeSet set, SpaceAsPlus space_as_plus)
    {
        StringBuilder builder;
        for(auto code_point : Utf8View(input))
        {
            if(space_as_plus == SpaceAsPlus::Yes && code_point == ' ')
                builder.append('+');
            else
                append_percent_encoded_if_necessary(builder, code_point, set);
        }
        return builder.to_string();
    }

    /**
     * @param input 
     * @return String 
     */
    String URL::percent_decode(StringView input)
    {
        if(!input.contains('%'))
            return input;
        StringBuilder builder;
        Utf8View utf8_view(input);
        for(auto it = utf8_view.begin(); !it.done(); ++it)
        {
            if(*it != '%')
            {
                builder.append_code_point(*it);
            }
            else if(!is_ascii_hex_digit(it.peek(1).value_or(0)) || !is_ascii_hex_digit(it.peek(2).value_or(0)))
            {
                builder.append_code_point(*it);
            }
            else
            {
                ++it;
                u8 byte = parse_ascii_hex_digit(*it) << 4;
                ++it;
                byte += parse_ascii_hex_digit(*it);
                builder.append(byte);
            }
        }
        return builder.to_string();
    }

} // namespace Mods