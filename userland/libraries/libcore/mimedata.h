/**
 * @file mimedata.h
 * @author Krisna Pranav
 * @brief mimedata
 * @version 6.0
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/hashmap.h>
#include <mods/url.h>
#include <libcore/object.h>

namespace Core 
{

    class MimeData : public Object 
    {
        C_OBJECT(MimeData);

    public:
        /**
         * @brief Destroy the MimeData object
         * 
         */
        virtual ~MimeData() = default;

        /**
         * @param mime_type 
         * @return ByteBuffer 
         */
        ByteBuffer data(String const& mime_type) const 
        { 
            return m_data.get(mime_type).value_or({}); 
        }

        /**
         * @brief Set the data object
         * 
         * @param mime_type 
         * @param data 
         */
        void set_data(String const& mime_type, ByteBuffer&& data) { m_data.set(mime_type, move(data)); }

        /**
         * @param mime_type 
         * @return true 
         * @return false 
         */
        bool has_format(String const& mime_type) const 
        { 
            return m_data.contains(mime_type); 
        }

        /**
         * @return Vector<String> 
         */
        Vector<String> formats() const;


        /**
         * @return true 
         * @return false 
         */
        bool has_text() const 
        { 
            return has_format("text/plain"); 
        }

        /**
         * @return String 
         */
        String text() const;

        void set_text(String const&);

        /**
         * @return true 
         * @return false 
         */
        bool has_urls() const 
        { 
            return has_format("text/uri-list"); 
        }

        /**
         * @return Vector<URL> 
         */
        Vector<URL> urls() const;

        void set_urls(Vector<URL> const&);

        /**
         * @return HashMap<String, ByteBuffer> const& 
         */
        HashMap<String, ByteBuffer> const& all_data() const 
        { 
            return m_data; 
        }

    private:
        /**
         * @brief Construct a new MimeData object
         * 
         */
        MimeData() = default;

        /**
         * @brief Construct a new MimeData object
         * 
         * @param data 
         */
        explicit MimeData(HashMap<String, ByteBuffer> const& data)
            : m_data(data)
        {
        }

        HashMap<String, ByteBuffer> m_data;
    }; // class MimeData : public Object

    /**
     * @return String 
     */
    String guess_mime_type_based_on_filename(StringView);

    /**
     * @return Optional<String> 
     */
    Optional<String> guess_mime_type_based_on_sniffed_bytes(ReadonlyBytes);

} // namespace Core
