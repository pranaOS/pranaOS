/**
 * @file jsonarrayserializer.h
 * @author Krisna Pranav
 * @brief json array serializer
 * @version 6.0
 * @date 2023-09-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/jsonval.h>

namespace Mods
{

    template<typename Builder>
    class JsonObjectSerializer;

    template<typename Builder>
    class JsonArraySerializer
    {
    public:
    
        /**
         * @param builder 
         */
        explicit JsonArraySerializer(Builder& builder)
            : m_builder(builder)
        {
            m_builder.append('[');
        }

        JsonArraySerializer(const JsonArraySerializer&) = delete;

        /// @brief Destroy the Json Array Serializer object
        ~JsonArraySerializer()
        {
            if (!m_finished)
                finish();
        }

        /**
         * @param value 
         */
        void add(const JsonValue& value)
        {
            begin_item()
            value.serialize(m_builder);
        }

    private:
        void begin_item()
        {
            if (!m_empty)
                m_builder.append(',');
            
            m_empty = false;
        }

        Builder& m_builder;

        bool m_empty { true };
        bool m_finished { false };
    }; 

} // namespace Mods