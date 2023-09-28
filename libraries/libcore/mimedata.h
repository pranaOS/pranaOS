/**
 * @file mimedata.h
 * @author Krisna Pranav
 * @brief mimedata
 * @version 6.0
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libcore/object.h>
#include <mods/byte_buffer.h>
#include <mods/hashmap.h>
#include <mods/url.h>

namespace Core 
{
    class MimeData : public Object 
    {
        C_OBJECT(MimeData);

    public:

        /// @brief Destroy the Mime Data object
        virtual ~MimeData() { }

        /**
         * @param mime_type 
         * @return ByteBuffer 
         */
        ByteBuffer data(const String& mime_type) const 
        { 
            return m_data.get(mime_type).value_or({}); 
        }

        /**
         * @param mime_type 
         * @param data 
         */
        void set_data(const String& mime_type, const ByteBuffer& data) 
        { 
            m_data.set(mime_type, data); 
        }

        /**
         * @param mime_type 
         * @return true 
         * @return false 
         */
        bool has_format(const String& mime_type) const 
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

        /// @brief Set the text object
        void set_text(const String&);

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

        /// @brief Set the urls object
        void set_urls(const Vector<URL>&);

        /**
         * @return const HashMap<String, ByteBuffer>& 
         */
        const HashMap<String, ByteBuffer>& all_data() const 
        { 
            return m_data; 
        }

    private:
        /// @brief Construct a new Mime Data object
        MimeData() { }

        /**
         * @param data 
         */
        explicit MimeData(const HashMap<String, ByteBuffer>& data)
            : m_data(data)
        {
        }

        HashMap<String, ByteBuffer> m_data;
    }; // class MimeData

    String guess_mime_type_based_on_filename(const StringView&);
} // namespace Core