/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/HashMap.h>
#include <base/URL.h>
#include <libweb/Forward.h>

namespace Web {

class LoadRequest {
public:
    LoadRequest()
    {
    }

    static LoadRequest create_for_url_on_page(const URL& url, Page* page);

    bool is_valid() const { return m_url.is_valid(); }

    const URL& url() const { return m_url; }
    void set_url(const URL& url) { m_url = url; }

    const String& method() const { return m_method; }
    void set_method(const String& method) { m_method = method; }

    const ByteBuffer& body() const { return m_body; }
    void set_body(const ByteBuffer& body) { m_body = body; }

    unsigned hash() const
    {
        return pair_int_hash(pair_int_hash(m_url.to_string().hash(), m_method.hash()), string_hash((const char*)m_body.data(), m_body.size()));
    }

    bool operator==(const LoadRequest& other) const
    {
        if (m_headers.size() != other.m_headers.size())
            return false;
        for (auto& it : m_headers) {
            auto jt = other.m_headers.find(it.key);
            if (jt == other.m_headers.end())
                return false;
            if (it.value != jt->value)
                return false;
        }
        return m_url == other.m_url && m_method == other.m_method && m_body == other.m_body;
    }

    void set_header(const String& name, const String& value) { m_headers.set(name, value); }
    String header(const String& name) const { return m_headers.get(name).value_or({}); }

    const HashMap<String, String>& headers() const { return m_headers; }

private:
    URL m_url;
    String m_method { "GET" };
    HashMap<String, String> m_headers;
    ByteBuffer m_body;
};

}

namespace Base {

template<>
struct Traits<Web::LoadRequest> : public GenericTraits<Web::LoadRequest> {
    static unsigned hash(const Web::LoadRequest& request) { return request.hash(); }
};

}
