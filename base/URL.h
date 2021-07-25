/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/StringView.h>

namespace Base {


class URL {
    friend class URLParser;

public:
        enum class PercentEncodeSet {
        C0Control,
        Fragment,
        Query,
        SpecialQuery,
        Path,
        Userinfo,
        Component,
        ApplicationXWWWFormUrlencoded,
        EncodeURI
    };

    enum class ExcludeFragment {
        No,
        Yes
    };

    URL() = default;
    URL(StringView const&);
    URL(char const* string)
        : URL(StringView(string))
    {
    }
    URL(String const& string)
        : URL(string.view())
    {
    }

        bool const& is_valid() const { return m_valid; }

    String const& scheme() const { return m_scheme; }
    String const& protocol() const { return m_scheme; }
    String const& username() const { return m_username; }
    String const& password() const { return m_password; }
    String const& host() const { return m_host; }
    Vector<String> const& paths() const { return m_paths; }
    String const& query() const { return m_query; }
    String const& fragment() const { return m_fragment; }
    u16 port() const { return m_port ? m_port : default_port_for_scheme(m_scheme); }
    bool const& cannot_be_a_base_url() const { return m_cannot_be_a_base_url; }

    bool includes_credentials() const { return !m_username.is_empty() || !m_password.is_empty(); }
    bool is_special() const { return is_special_scheme(m_scheme); }

private:
    URL(String&& data_mime_type, String&& data_payload, bool payload_is_base64)
        : m_valid(true)
        , m_scheme("data")
        , m_data_payload_is_base64(payload_is_base64)
        , m_data_mime_type(move(data_mime_type))
        , m_data_payload(move(data_payload))
    {
    }

    bool compute_validity() const;
    String serialize_data_url() const;

    static void append_percent_encoded_if_necessary(StringBuilder&, u32 code_point, PercentEncodeSet set = PercentEncodeSet::Userinfo);
    static void append_percent_encoded(StringBuilder&, u32 code_point);

    bool m_valid { false };
};

}
