/**
 * @file mimedata.cpp
 * @author Krisna Pranav
 * @brief mime data
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/lexicalpath.h>
#include <mods/stringbuilder.h>
#include <libcore/mimedata.h>

namespace Core
{
    
    /**
     * @return Vector<String> 
     */
    Vector<String> MimeData::formats() const
    {
        Vector<String> mime_types;
        mime_types.ensure_capacity(m_data.size());

        for (auto& it : m_data)
            mime_types.unchecked_append(it.key);

        return mime_types;
    }

    /**
     * @return Vector<URL> 
     */
    Vector<URL> MimeData::urls() const
    {
        auto it = m_data.find("text/uri-list");

        if (it == m_data.end())
            return {};
        
        Vector<URL> urls;

        for (auto& line : StringView(it->value).split_view('\n')) {
            urls.append(URL(line));
        }

        return urls;
    }

    /**
     * @param urls 
     */
    void MimeData::set_urls(Vector<URL> const& urls)
    {
        StringBuilder builder;
        for (auto& url : urls) {
            builder.append(url.to_string());
            builder.append('\n');
        }
        set_data("text/uri-list", builder.to_byte_buffer());
    }

    /**
     * @return String 
     */
    String MimeData::text() const
    {
        return String::copy(m_data.get("text/plain").value_or({}));
    }

    /**
     * @param text 
     */
    void MimeData::set_text(String const& text)
    {
        set_data("text/plain", text.to_byte_buffer());
    }
} // namespace Core
