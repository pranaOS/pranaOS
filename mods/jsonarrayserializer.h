/**
 * @file jsonarrayserializer.h
 * @author Krisna Pranav
 * @brief json array serializer
 * @version 6.0
 * @date 2023-09-08
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/jsonval.h>

namespace Mods 
{

    /**
     * @tparam Builder 
     */
    template<typename Builder>
    class JsonObjectSerializer;

    /**
     * @tparam Builder 
     */

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

        /// @brief Construct a new Json Array Serializer object
        JsonArraySerializer(const JsonArraySerializer&) = delete;
        JsonArraySerializer(JsonArraySerializer&&) = delete;

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
            begin_item();
            value.serialize(m_builder);
        }

        /**
         * @param value 
         */
        void add(const StringView& value)
        {
            begin_item();
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @param value 
         */
        void add(const String& value)
        {
            begin_item();
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @param value 
         */
        void add(const char* value)
        {
            begin_item();
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @return JsonArraySerializer<Builder> 
         */
        JsonArraySerializer<Builder> add_array()
        {
            begin_item();
            return JsonArraySerializer(m_builder);
        }

        /**
         * @return JsonObjectSerializer<Builder> 
         */
        JsonObjectSerializer<Builder> add_object();

        void finish()
        {
            ASSERT(!m_finished);
            m_finished = true;
            m_builder.append(']');
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
    }; // class JsonArraySerializer

} // namespace Mods

using Mods::JsonArraySerializer;
