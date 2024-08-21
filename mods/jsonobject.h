/**
 * @file jsonobject.h
 * @author Krisna Pranav
 * @brief json object
 * @version 6.0
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashmap.h>
#include <mods/jsonarray.h>
#include <mods/jsonobjectserialize.h>
#include <mods/jsonval.h>
#include <mods/string.h>
#include <mods/string_impl.h>

namespace Mods 
{

    class JsonObject 
    {
    public:
        /// @brief Construct a new Json Object object
        JsonObject() { }
        ~JsonObject() { }

        /**
         * @param other 
         */
        JsonObject(const JsonObject& other)
            : m_order(other.m_order)
            , m_members(other.m_members)
        { }     

        /**
         * @param other 
         */
        JsonObject(JsonObject&& other)
            : m_order(move(other.m_order))
            , m_members(move(other.m_members))
        { }

        /**
         * @param other 
         * @return JsonObject& 
         */
        JsonObject& operator=(const JsonObject& other)
        {
            if (this != &other) {
                m_members = other.m_members;
                m_order = other.m_order;
            }

            return *this;
        }

        /**
         * @param other 
         * @return JsonObject& 
         */
        JsonObject& operator=(JsonObject&& other)
        {
            if (this != &other) {
                m_members = move(other.m_members);
                m_order = move(other.m_order);
            }

            return *this;
        }

        /**
         * @return int 
         */
        int size() const 
        { 
            return m_members.size(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return m_members.is_empty(); 
        }

        /**
         * @param key 
         * @return JsonValue 
         */
        JsonValue get(const String& key) const
        {
            auto* value = get_ptr(key);
            return value ? *value : JsonValue(JsonValue::Type::Null);
        }

        /**
         * @param key 
         * @param alternative 
         * @return JsonValue 
         */
        JsonValue get_or(const String& key, JsonValue alternative) const
        {
            auto* value = get_ptr(key);
            return value ? *value : alternative;
        }

        /**
         * @param key 
         * @return const JsonValue* 
         */
        const JsonValue* get_ptr(const String& key) const
        {
            auto it = m_members.find(key);
            if (it == m_members.end())
                return nullptr;

            return &(*it).value;
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool has(const String& key) const
        {
            return m_members.contains(key);
        }

        /**
         * @param key 
         * @param value 
         */
        void set(const String& key, JsonValue value)
        {
            if (m_members.set(key, move(value)) == HashSetResult::ReplacedExistingEntry)
                m_order.remove(m_order.find_first_index(key).value());
            m_order.append(key);
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void for_each_member(Callback callback) const
        {
            for (size_t i = 0; i < m_order.size(); ++i) {
                auto property = m_order[i];
                callback(property, m_members.get(property).value());
            }
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(const String& key)
        {
            if (m_members.remove(key)) {
                m_order.remove(m_order.find_first_index(key).value());
                return true;
            }
            return false;
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

    private:
        Vector<String> m_order;
        HashMap<String, JsonValue> m_members;
    }; // class JsonObject

    /**
     * @tparam Builder 
     * @param builder 
     */
    template<typename Builder>
    inline void JsonObject::serialize(Builder& builder) const
    {
        JsonObjectSerializer serializer { builder };
        for_each_member([&](auto& key, auto& value) {
            serializer.add(key, value);
        });
    }

    /**
     * @tparam Builder 
     * @return Builder::OutputType 
     */
    template<typename Builder>
    inline typename Builder::OutputType JsonObject::serialized() const
    {
        Builder builder;
        serialize(builder);
        return builder.build();
    }

    /**
     * @tparam Builder 
     * @param builder 
     */
    template<typename Builder>
    inline void JsonValue::serialize(Builder& builder) const
    {
        switch (m_type) {
        case Type::String: {
            builder.append("\"");
            builder.append_escaped_for_json({ m_value.as_string->characters(), m_value.as_string->length() });
            builder.append("\"");
        } break;
        case Type::Array:
            m_value.as_array->serialize(builder);
            break;
        case Type::Object:
            m_value.as_object->serialize(builder);
            break;
        case Type::Bool:
            builder.append(m_value.as_bool ? "true" : "false");
            break;
    #if !defined(KERNEL)
        case Type::Double:
            builder.appendf("%g", m_value.as_double);
            break;
    #endif
        case Type::Int32:
            builder.appendff("{}", as_i32());
            break;
        case Type::Int64:
            builder.appendff("{}", as_i64());
            break;
        case Type::UnsignedInt32:
            builder.appendff("{}", as_u32());
            break;
        case Type::UnsignedInt64:
            builder.appendff("{}", as_u64());
            break;
        case Type::Null:
            builder.append("null");
            break;
        default:
            ASSERT_NOT_REACHED();
        }
    }

    /**
     * @tparam Builder 
     * @return Builder::OutputType 
     */
    template<typename Builder>
    inline typename Builder::OutputType JsonValue::serialized() const
    {
        Builder builder;
        serialize(builder);
        return builder.build();
    }

} // namespace Mods

using Mods::JsonObject;
