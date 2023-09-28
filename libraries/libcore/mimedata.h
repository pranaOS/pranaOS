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
    public:
        /// @brief Destroy the Mime Data object
        virtual ~MimeData() {}

        /**
         * @param mime_type 
         * @return ByteBuffer 
         */
        ByteBuffer data(const String& mime_type) const
        {
            return m_data.get(mime_type).value_or({});
        }

    private:
        MimeData() {}

        /**
         * @param data 
         */
        explicit MimeData(const HashMap<String, ByteBuffer>& data)
            : m_data(data)
        {}

        HashMap<String, ByteBuffer> m_data;
    }; // class MimeData

    String guess_mime_type_based_on_filename(const StringView&);
}