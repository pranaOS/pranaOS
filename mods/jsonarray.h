/**
 * @file jsonarray.h
 * @author Krisna Pranav
 * @brief json array
 * @version 6.0
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/jsonarrayserializer.h>
#include <mods/jsonval.h>
#include <mods/vector.h>

namespace Mods 
{

    class JsonArray 
    {
    public:
        /// @brief Construct a new Json Array object
        JsonArray() { }
        ~JsonArray() { }

        /**
         * @param other 
         */
        JsonArray(const JsonArray& other)
            : m_values(other.m_values)
        {
        }

        /**
         * @param other 
         */
        JsonArray(JsonArray&& other)
            : m_values(move(other.m_values))
        {
        }

        /**
         * @param other 
         * @return JsonArray& 
         */
        JsonArray& operator=(const JsonArray& other)
        {
            if (this != &other)
                m_values = other.m_values;

            return *this;
        }

        /**
         * @param other 
         * @return JsonArray& 
         */
        JsonArray& operator=(JsonArray&& other)
        {
            if (this != &other)
                m_values = move(other.m_values);

            return *this;
        }

        /**
         * @return int 
         */
        int size() const 
        { 
            return m_values.size(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return m_values.is_empty(); 
        }

        /**
         * @param index 
         * @return const JsonValue& 
         */
        const JsonValue& at(int index) const 
        { 
            return m_values.at(index); 
        }

        /**
         * @param index 
         * @return const JsonValue& 
         */
        const JsonValue& operator[](int index) const 
        { 
            return at(index); 
        }

        /// @breif: clear
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
         * @tparam Builder 
         * @return Builder::OutputType 
         */
        template<typename Builder>
        typename Builder::OutputType serialized() const;

        /**
         * @tparam Builder 
         */
        template<typename Builder>
        void serialize(Builder&) const;

        /**
         * @return String 
         */
        String to_string() const 
        { 
            return serialized<StringBuilder>(); 
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void for_each(Callback callback) const
        {
            for (auto& value : m_values)
                callback(value);
        }

        /**
         * @return const Vector<JsonValue>& 
         */
        const Vector<JsonValue>& values() const 
        { 
            return m_values; 
        }

        /**
         * @param capacity 
         */
        void ensure_capacity(int capacity) 
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
    template<typename Builder>
    inline void JsonArray::serialize(Builder& builder) const
    {
        JsonArraySerializer serializer { builder };
        for_each([&](auto& value) { serializer.add(value); });
    }
    
    /**
     * @tparam Builder 
     * @return Builder::OutputType 
     */
    template<typename Builder>
    inline typename Builder::OutputType JsonArray::serialized() const
    {
        Builder builder;
        serialize(builder);
        return builder.build();
    }

} // namespace Mods

using Mods::JsonArray;
