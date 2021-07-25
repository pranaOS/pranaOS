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

}