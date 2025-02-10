/**
 * @file jsonobjectserialize.h
 * @author Krisna Pranav
 * @brief json object serialize
 * @version 6.0
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/error.h>
#include <mods/jsonarrayserializer.h>
#include <mods/try.h>

#ifndef KERNEL
#include <mods/jsonvalue.h>
#endif

namespace Mods
{
    /**
     * @tparam Builder 
     */
    template <typename Builder>
    class JsonObjectSerializer
    {
    public:
        /**
         * @param builder 
         * @return ErrorOr<JsonObjectSerializer> 
         */
        static ErrorOr<JsonObjectSerializer> try_create(Builder& builder)
        {
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(builder.try_append('{'));
            else
                TRY(builder.append('{'));
            return JsonObjectSerializer{builder};
        }

        /**
         * @brief Construct a new Json Object Serializer object
         * 
         * @param other 
         */
        JsonObjectSerializer(JsonObjectSerializer&& other)
            : m_builder(other.m_builder), m_empty(other.m_empty), m_finished(exchange(other.m_finished, true))
        {
        }

        /**
         * @brief Construct a new Json Object Serializer object
         * 
         */
        JsonObjectSerializer(JsonObjectSerializer const&) = delete;

        /**
         * @brief Destroy the Json Object Serializer object
         * 
         */
        ~JsonObjectSerializer()
        {
            VERIFY(m_finished);
        }

    #ifndef KERNEL
        ErrorOr<void> add(StringView key, JsonValue const& value)
        {
            TRY(begin_item(key));
            value.serialize(m_builder);
            return {};
        }
    #endif

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, StringView value)
        {
            TRY(begin_item(key));

            if constexpr(IsLegacyBuilder<Builder>)
            {
                TRY(m_builder.try_append('"'));
                TRY(m_builder.try_append_escaped_for_json(value));
                TRY(m_builder.try_append('"'));
            }
            else
            {
                TRY(m_builder.append('"'));
                TRY(m_builder.append_escaped_for_json(value));
                TRY(m_builder.append('"'));
            }
            return {};
        }

    #ifndef KERNEL
        ErrorOr<void> add(StringView key, String const& value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
            {
                TRY(m_builder.try_append('"'));
                TRY(m_builder.try_append_escaped_for_json(value));
                TRY(m_builder.try_append('"'));
            }
            else
            {
                TRY(m_builder.append('"'));
                TRY(m_builder.append_escaped_for_json(value));
                TRY(m_builder.append('"'));
            }
            return {};
        }
    #endif

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, char const* value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
            {
                TRY(m_builder.try_append('"'));
                TRY(m_builder.try_append_escaped_for_json(value));
                TRY(m_builder.try_append('"'));
            }
            else
            {
                TRY(m_builder.append('"'));
                TRY(m_builder.append_escaped_for_json(value));
                TRY(m_builder.append('"'));
            }
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, bool value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_append(value ? "true" : "false"));
            else
                TRY(m_builder.append(value ? "true" : "false"));
            return {};
        }   

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, int value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, unsigned value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, long value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, long unsigned value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, long long value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView key, long long unsigned value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

    #ifndef KERNEL
        ErrorOr<void> add(StringView key, float value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        ErrorOr<void> add(StringView key, double value)
        {
            TRY(begin_item(key));
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }
    #endif

        /**
         * @param key 
         * @return ErrorOr<JsonArraySerializer<Builder>> 
         */
        ErrorOr<JsonArraySerializer<Builder>> add_array(StringView key)
        {
            TRY(begin_item(key));
            return JsonArraySerializer<Builder>::try_create(m_builder);
        }

        /**
         * @param key 
         * @return ErrorOr<JsonObjectSerializer<Builder>> 
         */
        ErrorOr<JsonObjectSerializer<Builder>> add_object(StringView key)
        {
            TRY(begin_item(key));
            return JsonObjectSerializer::try_create(m_builder);
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> finish()
        {
            VERIFY(!m_finished);
            m_finished = true;
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_append('}'));
            else
                TRY(m_builder.append('}'));
            return {};
        }

    private:
        /**
         * @brief Construct a new Json Object Serializer object
         * 
         * @param builder 
         */
        explicit JsonObjectSerializer(Builder& builder)
            : m_builder(builder)
        {
        }

        /**
         * @param key 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> begin_item(StringView key)
        {
            VERIFY(!m_finished);
            if(!m_empty)
            {
                if constexpr(IsLegacyBuilder<Builder>)
                    TRY(m_builder.try_append(','));
                else
                    TRY(m_builder.append(','));
            }
            m_empty = false;

            if constexpr(IsLegacyBuilder<Builder>)
            {
                TRY(m_builder.try_append('"'));
                TRY(m_builder.try_append_escaped_for_json(key));
                TRY(m_builder.try_append("\":"));
            }
            else
            {
                TRY(m_builder.append('"'));
                TRY(m_builder.append_escaped_for_json(key));
                TRY(m_builder.append("\":"));
            }
            return {};
        }

        Builder& m_builder;
        bool m_empty{true};
        bool m_finished{false};
    }; // class JsonObjectSerializer

    /**
     * @tparam  
     */
    template <>
    struct JsonObjectSerializer<void>
    {
        /**
         * @tparam Builder 
         * @param builder 
         * @return ErrorOr<JsonObjectSerializer<Builder>> 
         */
        template <typename Builder>
        static ErrorOr<JsonObjectSerializer<Builder>> try_create(Builder& builder)
        {
            return JsonObjectSerializer<Builder>::try_create(builder);
        }
    }; // struct JsonObjectSerializer<void>

    /**
     * @tparam Builder 
     * @return ErrorOr<JsonObjectSerializer<Builder>> 
     */
    template <typename Builder>
    ErrorOr<JsonObjectSerializer<Builder>> JsonArraySerializer<Builder>::add_object()
    {
        TRY(begin_item());
        return JsonObjectSerializer<Builder>::try_create(m_builder);
    } // ErrorOr<JsonObjectSerializer<Builder>> JsonArraySerializer<Builder>::add_object()

} // namespace Mods

using Mods::JsonObjectSerializer;
