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
        URL() {}
        URL(const char* string) : URL(StringView(string)) {}

        URL(const String& string) : URL(string.view()) {}

        bool is_valid() const
        {
            return m_valid;
        }

        String protocol() const
        {
            return m_protocol;
        }

        String host() const
        {
            return m_host;            
        }

        String path() const
        {
            return m_path;
        }

        String query() const
        {
            return m_query;
        }

        String fragment() const
        {
            return m_fragment;
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
} // namespace Mods