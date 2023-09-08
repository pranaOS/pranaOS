/**
 * @file jsonobjectserialize.h
 * @author Krisna Pranav
 * @brief json object serialize
 * @version 6.0
 * @date 2023-09-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/jsonarrayserializer.h>
#include <mods/jsonval.h>

namespace Mods
{
    
    template<typename Builder>
    class JsonObjectSerializer
    {
    public:
        /**
         * @param builder 
         */
        explicit JsonObjectSerializer(Builder& builder)
            : m_builder(builder)
        {
            m_builder.append('{');
        }

        JsonObjectSerializer(const JsonObjectSerializer&) = delete;
        JsonObjectSerializer(JsonObjectSerializer&&) = delete;

        /// @brief Destroy the Json Object Serializer object
        ~JsonObjectSerializer()
        {
            if (!m_finished)
                finish();
        }

        void finish()
        {
            ASSERT(!m_finished);

            m_finished = true;

            m_builder.append('}');
        }

    private:
        /**
         * @param key 
         */
        void begin_item(const StringView& key)
        {
            if (!m_empty)
                m_builder.append(',');
            
            m_empty = false;

            m_builder.append('"');
            m_builder.append_escaped_for_json(key);
            m_builder.append("\":");
        }

        Builder& m_builder;

        bool m_empty { true };
        bool m_finished { false };
    }; // class JsonObjectSerializer

} // namespace Mods