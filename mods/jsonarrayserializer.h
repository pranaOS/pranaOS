/**
 * @file jsonarrayserializer.h
 * @author Krisna Pranav
 * @brief json array serializer
 * @version 6.0
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/error.h>
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
    inline constexpr bool IsLegacyBuilder = requires(Builder builder) 
    { 
        builder.try_append('\0'); 
    };

    /**
     * @tparam Builder 
     */
    template <typename Builder = void>
    class JsonObjectSerializer;

    /**
     * @tparam Builder 
     */
    template <typename Builder = void>
    class JsonArraySerializer
    {
    public:
        /**
         * @param builder 
         * @return ErrorOr<JsonArraySerializer> 
         */
        static ErrorOr<JsonArraySerializer> try_create(Builder& builder)
        {
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(builder.try_append('['));
            else
                TRY(builder.append('['));

            return JsonArraySerializer{builder};
        }

        /**
         * @brief Construct a new Json Array Serializer object
         * 
         * @param other 
         */
        JsonArraySerializer(JsonArraySerializer&& other)
            : m_builder(other.m_builder), m_empty(other.m_empty), m_finished(exchange(other.m_finished, true))
        {
        }

        /**
         * @brief Construct a new Json Array Serializer object
         * 
         */
        JsonArraySerializer(JsonArraySerializer const&) = delete;

        /**
         * @brief Destroy the Json Array Serializer object
         * 
         */
        ~JsonArraySerializer()
        {
            VERIFY(m_finished);
        }

    #ifndef KERNEL
        ErrorOr<void> add(JsonValue const& value)
        {
            TRY(begin_item());
            value.serialize(m_builder);
            return {};
        }
    #endif

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(StringView value)
        {
            TRY(begin_item());
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
        ErrorOr<void> add(String const& value)
        {
            TRY(begin_item());
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
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(char const* value)
        {
            TRY(begin_item());
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
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(bool value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_append(value ? "true"sv : "false"sv));
            else
                TRY(m_builder.append(value ? "true"sv : "false"sv));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(int value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(unsigned value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(long value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(long unsigned value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(long long value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> add(long long unsigned value)
        {
            TRY(begin_item());
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_appendff("{}", value));
            else
                TRY(m_builder.appendff("{}", value));
            return {};
        }

        /**
         * @return ErrorOr<JsonArraySerializer<Builder>> 
         */
        ErrorOr<JsonArraySerializer<Builder>> add_array()
        {
            TRY(begin_item());
            return JsonArraySerializer::try_create(m_builder);
        }

        /**
         * @return ErrorOr<JsonObjectSerializer<Builder>> 
         */
        ErrorOr<JsonObjectSerializer<Builder>> add_object();

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> finish()
        {
            VERIFY(!m_finished);
            m_finished = true;
            if constexpr(IsLegacyBuilder<Builder>)
                TRY(m_builder.try_append(']'));
            else
                TRY(m_builder.append(']'));
            return {};
        }

    private:
        /**
         * @brief Construct a new Json Array Serializer object
         * 
         * @param builder 
         */
        explicit JsonArraySerializer(Builder& builder)
            : m_builder(builder)
        {
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> begin_item()
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
            return {};
        }

        Builder& m_builder;
        bool m_empty{true};
        bool m_finished{false};
    }; // class JsonArraySerializer

    /**
     * @tparam  
     */
    template <>
    struct JsonArraySerializer<void>
    {
        template <typename Builder>
        static ErrorOr<JsonArraySerializer<Builder>> try_create(Builder& builder)
        {
            return JsonArraySerializer<Builder>::try_create(builder);
        }
    };

} // namespace Mods

using Mods::JsonArraySerializer;
