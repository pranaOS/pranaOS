/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/CharacterTypes.h>
#include <base/Debug.h>
#include <base/LexicalPath.h>
#include <base/StringBuilder.h>
#include <base/URL.h>
#include <base/URLParser.h>
#include <base/Utf8View.h>

namespace Base {

URL::URL(StringView const& string)
    : URL(URLParser::parse({}, string))
{
    if constexpr (URL_PARSER_DEBUG) {
        if (m_valid)
            dbgln("URL constructor: Parsed URL to be '{}'.", serialize());
        else
            dbgln("URL constructor: Parsed URL to be invalid.");
    }
}

String URL::path() const
{
    if (cannot_be_a_base_url())
        return paths()[0];
    StringBuilder builder;
    for (auto& path : m_paths) {
        builder.append('/');
        builder.append(path);
    }

    return builder.to_string();
}

URL URL::complete_url(String const& string) const
{
    if (!is_valid())
        return {};
    
    return URLParser::parse({}, string, this);
}

void URL::set_scheme(String scheme)
{
    m_scheme = move(scheme);
    m_valid = compute_validity();
}

void URL::set_username(String username)
{
    m_username = move(username);
    m_valid = compute_validity();
}

void URL::set_password(String password)
{
    m_password = move(password);
    m_valid = compute_validity();
}

void URL::set_host(String host)
{
    m_host = move(host);
    m_valid = compute_validity();
}

void URL::set_port(u16 port)
{
    if (port == default_port_for_scheme(m_scheme)) {
        m_port = 0;
        return;
    }
    m_port = port;
    m_valid = compute_validity();
}

void URL::set_paths(Vector<String> paths)
{
    m_paths = move(paths);
    m_valid = compute_validity();
}

void URL::set_query(String query)
{
    m_query = move(query);
}

void URL::set_fragment(String fragment)
{
    m_fragment = move(fragment);
}

bool URL::compute_validity() const
{
    if (m_scheme.is_empty())
        return false;

    if (m_scheme == "data") {
        if (m_data_mime_type.is_empty())
            return false;
        if (m_data_payload_is_base64) {
            if (m_data_payload.length() % 4 != 0)
                return false;
            for (auto character : m_data_payload) {
                if (!is_ascii_alphanumeric(character) || character == '+' || character == '/' || character == '=')
                    return false;
            }
        }
    } else if (m_cannot_be_a_base_url) {
        if (m_paths.size() != 1)
            return false;
        if (m_paths[0].is_empty())
            return false;
    } else {
        if (m_scheme.is_one_of("about", "mailto"))
            return false;

        if (m_paths.size() == 0)
            return false;
    }

    if (m_scheme == "file" && m_host.is_null())
        return false;

    return true;
}

bool URL::scheme_requires_port(StringView const& scheme)
{
    return (default_port_for_scheme(scheme) != 0);
}

u16 URL::default_port_for_scheme(StringView const& scheme)
{
    if (scheme == "http")
        return 80;
    if (scheme == "https")
        return 443;
    if (scheme == "gemini")
        return 1965;
    if (scheme == "irc")
        return 6667;
    if (scheme == "ircs")
        return 6697;
    if (scheme == "ws")
        return 80;
    if (scheme == "wss")
        return 443;
    return 0;
}

URL URL::create_with_file_scheme(String const& path, String const& fragment, String const& hostname)
{
    LexicalPath lexical_path(path);
    if (!lexical_path.is_absolute())
        return {};

    URL url;
    url.set_scheme("file");

    url.set_host(hostname.is_null() || hostname == "localhost" ? String::empty() : hostname);
    url.set_paths(lexical_path.parts());

    if (path.ends_with('/'))
        url.append_path("");
    url.set_fragment(fragment);
    return url;
}

URL URL::create_with_url_or_path(String const& url_or_path)
{
    URL url = url_or_path;
    if (url.is_valid())
        return url;

    String path = LexicalPath::canonicalized_path(url_or_path);
    return URL::create_with_file_scheme(path);
}

bool URL::is_special_scheme(StringView const& scheme)
{
    return scheme.is_one_of("ftp", "file", "http", "https", "ws", "wss");
}

String URL::serialize_data_url() const
{
    VERIFY(m_scheme == "data");
    VERIFY(!m_data_mime_type.is_null());
    VERIFY(!m_data_payload.is_null());
    StringBuilder builder;
    builder.append(m_scheme);
    builder.append(':');
    builder.append(m_data_mime_type);
    if (m_data_payload_is_base64)
        builder.append(";base64");
    builder.append(',');
    builder.append(URL::percent_encode(m_data_payload, PercentEncodeSet::C0Control));
    return builder.to_string();
}

String URL::serialize(ExcludeFragment exclude_fragment) const
{
    if (m_scheme == "data")
        return serialize_data_url();
    StringBuilder builder;
    builder.append(m_scheme);
    builder.append(':');

    if (!m_host.is_null()) {
        builder.append("//");

        if (includes_credentials()) {
            builder.append(percent_encode(m_username, PercentEncodeSet::Userinfo));
            if (!m_password.is_empty()) {
                builder.append(':');
                builder.append(percent_encode(m_password, PercentEncodeSet::Userinfo));
            }
            builder.append('@');
        }

        builder.append(m_host);
        if (m_port != 0)
            builder.appendff(":{}", m_port);
    }

    if (cannot_be_a_base_url()) {
        builder.append(percent_encode(m_paths[0], PercentEncodeSet::Path));
    } else {
        if (m_host.is_null() && m_paths.size() > 1 && m_paths[0].is_empty())
            builder.append("/.");
        for (auto& segment : m_paths) {
            builder.append('/');
            builder.append(percent_encode(segment, PercentEncodeSet::Path));
        }
    }

    if (!m_query.is_null()) {
        builder.append('?');
        builder.append(percent_encode(m_query, is_special() ? URL::PercentEncodeSet::SpecialQuery : URL::PercentEncodeSet::Query));
    }

    if (exclude_fragment == ExcludeFragment::No && !m_fragment.is_null()) {
        builder.append('#');
        builder.append(percent_encode(m_fragment, PercentEncodeSet::Fragment));
    }

    return builder.to_string();
}


}