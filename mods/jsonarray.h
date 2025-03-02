/**
 * @file jsonarray.h
 * @author Krisna Pranav
 * @brief json array
 * @version 6.0
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/concept.h>
#include <mods/jsonarrayserializer.h>
#include <mods/jsonvalue.h>
#include <mods/vector.h>

namespace Mods
{
    class JsonArray
    {
    public:
        /**
         * @brief Construct a new Json Array object
         *
         */
        JsonArray() = default;

        /**
         * @brief Destroy the Json Array object
         *
         */
        ~JsonArray() = default;

        /**
         * @brief Construct a new Json Array object
         *
         * @param other
         */
        JsonArray(JsonArray const& other)
            : m_values(other.m_values)
        {
        }

        /**
         * @brief Construct a new Json Array object
         *
         * @param other
         */
        JsonArray(JsonArray&& other)
            : m_values(move(other.m_values))
        {
        }

        /**
         * @brief Construct a new Json Array object
         *
         * @tparam ContainerT
         * @param source
         */
        template <IterableContainer ContainerT>
        JsonArray(ContainerT const& source)
        {
            for(auto& value : source)
                m_values.append(move(value));
        }

        /**
         * @param other
         * @return JsonArray&
         */
        JsonArray& operator=(JsonArray const& other)
        {
            if(this != &other)
                m_values = other.m_values;
            return *this;
        }

        /**
         * @param other
         * @return JsonArray&
         */
        JsonArray& operator=(JsonArray&& other)
        {
            if(this != &other)
                m_values = move(other.m_values);
            return *this;
        }

        /**
         * @return size_t
         */
        [[nodiscard]] size_t size() const
        {
            return m_values.size();
        }

        /**
         * @return true
         * @return false
         */
        [[nodiscard]] bool is_empty() const
        {
            return m_values.is_empty();
        }

        /**
         * @param index
         * @return JsonValue const&
         */
        [[nodiscard]] JsonValue const& at(size_t index) const
        {
            return m_values.at(index);
        }

        /**
         * @param index
         * @return JsonValue const&
         */
        [[nodiscard]] JsonValue const& operator[](size_t index) const
        {
            return at(index);
        }

        void clear()
        {
            m_values.clear();
        }

        /**
         * @param value
         */
        void append(JsonValue value)
        {
            m_values.append(move(value));
        }

        /**
         * @param index
         * @param value
         */
        void set(size_t index, JsonValue value)
        {
            m_values[index] = move(value);
        }

        /**
         * @tparam Builder
         * @return Builder::OutputType
         */
        template <typename Builder>
        typename Builder::OutputType serialized() const;

        /**
         * @tparam Builder
         */
        template <typename Builder>
        void serialize(Builder&) const;

        /**
         * @return String
         */
        [[nodiscard]] String to_string() const
        {
            return serialized<StringBuilder>();
        }

        /**
         * @tparam Callback
         * @param callback
         */
        template <typename Callback>
        void for_each(Callback callback) const
        {
            for(auto const& value : m_values)
                callback(value);
        }

        /**
         * @return Vector<JsonValue> const&
         */
        [[nodiscard]] Vector<JsonValue> const& values() const
        {
            return m_values;
        }

        /**
         * @param capacity
         */
        void ensure_capacity(size_t capacity)
        {
            m_values.ensure_capacity(capacity);
        }

    private:
        Vector<JsonValue> m_values;
    }; // class JsonArray

    /**
     * @tparam Builder
     * @param builder
     */
    template <typename Builder>
    inline void JsonArray::serialize(Builder& builder) const
    {
        auto serializer = MUST(JsonArraySerializer<>::try_create(builder));
        for_each([&](auto& value)
                { MUST(serializer.add(value)); });
        MUST(serializer.finish());
    }

    /**
     * @tparam Builder
     * @return Builder::OutputType
     */
    template <typename Builder>
    inline typename Builder::OutputType JsonArray::serialized() const
    {
        Builder builder;
        serialize(builder);
        return builder.build();
    }

} // namespace Mods

using Mods::JsonArray;
