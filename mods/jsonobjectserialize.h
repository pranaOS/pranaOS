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
    /**
     * @tparam Builder 
     */
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

        /// @brief Construct a new Json Object Serializer object
        JsonObjectSerializer(const JsonObjectSerializer&) = delete;
        JsonObjectSerializer(JsonObjectSerializer&&) = delete;

        /// @brief Destroy the Json Object Serializer object
        ~JsonObjectSerializer()
        {
            if (!m_finished)
                finish();
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, const JsonValue& value)
        {
            begin_item(key);
            value.serialize(m_builder);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, const StringView& value)
        {
            begin_item(key);
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, const String& value)
        {
            begin_item(key);
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, const char* value)
        {
            begin_item(key);
            m_builder.append('"');
            m_builder.append_escaped_for_json(value);
            m_builder.append('"');
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, bool value)
        {
            begin_item(key);
            m_builder.append(value ? "true" : "false");
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, int value)
        {
            begin_item(key);
            m_builder.appendf("%d", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, unsigned value)
        {
            begin_item(key);
            m_builder.appendf("%u", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, long value)
        {
            begin_item(key);
            m_builder.appendf("%ld", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, long unsigned value)
        {
            begin_item(key);
            m_builder.appendf("%lu", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, long long value)
        {
            begin_item(key);
            m_builder.appendf("%lld", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, long long unsigned value)
        {
            begin_item(key);
            m_builder.appendf("%llu", value);
        }

        /**
         * @param key 
         * @param value 
         */
        void add(const StringView& key, double value)
        {
            begin_item(key);
            m_builder.appendf("%f", value);
        }

        /**
         * @param key 
         * @return JsonArraySerializer<Builder> 
         */
        JsonArraySerializer<Builder> add_array(const StringView& key)
        {
            begin_item(key);
            return JsonArraySerializer(m_builder);
        }

        /**
         * @param key 
         * @return JsonObjectSerializer<Builder> 
         */
        JsonObjectSerializer<Builder> add_object(const StringView& key)
        {
            begin_item(key);
            return JsonObjectSerializer(m_builder);
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
    };

    template<typename Builder>
    JsonObjectSerializer<Builder> JsonArraySerializer<Builder>::add_object()
    {
        begin_item();
        return JsonObjectSerializer(m_builder);
    }

} // namespace Mods

using Mods::JsonObjectSerializer;
