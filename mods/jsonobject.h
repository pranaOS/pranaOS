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
#include <mods/jsonvalue.h>
#include <mods/string.h>

namespace Mods
{
    class JsonObject
    {
    public:
        /**
         * @brief Construct a new Json Object object
         * 
         */
        JsonObject() = default;

        /**
         * @brief Destroy the Json Object object
         * 
         */
        ~JsonObject() = default;

        /**
         * @brief Construct a new Json Object object
         * 
         * @param other 
         */
        JsonObject(JsonObject const& other)
            : m_members(other.m_members)
        {
        }

        /**
         * @brief Construct a new Json Object object
         * 
         * @param other 
         */
        JsonObject(JsonObject&& other)
            : m_members(move(other.m_members))
        {
        }

        /**
         * @param other 
         * @return JsonObject& 
         */
        JsonObject& operator=(JsonObject const& other)
        {
            if(this != &other)
                m_members = other.m_members;
            return *this;
        }

        /**
         * @param other 
         * @return JsonObject& 
         */
        JsonObject& operator=(JsonObject&& other)
        {
            if(this != &other)
                m_members = move(other.m_members);
            return *this;
        }

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t size() const
        {
            return m_members.size();
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const
        {
            return m_members.is_empty();
        }

        /**
         * @param key 
         * @return JsonValue const& 
         */
        [[nodiscard]] JsonValue const& get(StringView key) const
        {
            auto const* value = get_ptr(key);
            static JsonValue* s_null_value{nullptr};
            if(!value)
            {
                if(!s_null_value)
                    s_null_value = new JsonValue;
                return *s_null_value;
            }
            return *value;
        }

        /**
         * @brief Get the ptr object
         * 
         * @param key 
         * @return JsonValue const* 
         */
        [[nodiscard]] JsonValue const* get_ptr(StringView key) const
        {
            auto it = m_members.find(key);
            if(it == m_members.end())
                return nullptr;
            return &(*it).value;
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] [[nodiscard]] bool has(StringView key) const
        {
            return m_members.contains(key);
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_null(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_null();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_bool(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_bool();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_string(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_string();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_i32(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_i32();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_u32(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_u32();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_i64(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_i64();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_u64(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_u64();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_number(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_number();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_array(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_array();
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool has_object(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_object();
        }
    #ifndef KERNEL
        [[nodiscard]] [[nodiscard]] bool has_double(StringView key) const
        {
            auto const* value = get_ptr(key);
            return value && value->is_double();
        }
    #endif

        /**
         * @param key 
         * @param value 
         */
        void set(String const& key, JsonValue value)
        {
            m_members.set(key, move(value));
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template <typename Callback>
        void for_each_member(Callback callback) const
        {
            for(auto const& member : m_members)
                callback(member.key, member.value);
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(StringView key)
        {
            return m_members.remove(key);
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

    private:
        OrderedHashMap<String, JsonValue> m_members;
    }; // class JsonObject

    /**
     * @tparam Builder 
     * @param builder 
     */
    template <typename Builder>
    inline void JsonObject::serialize(Builder& builder) const
    {
        auto serializer = MUST(JsonObjectSerializer<>::try_create(builder));
        for_each_member([&](auto& key, auto& value)
                        { MUST(serializer.add(key, value)); });
        MUST(serializer.finish());
    }

    /**
     * @tparam Builder 
     * @return Builder::OutputType 
     */
    template <typename Builder>
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
    template <typename Builder>
    inline void JsonValue::serialize(Builder& builder) const
    {
        switch(m_type)
        {
        case Type::String:
        {
            builder.append("\"");
            builder.append_escaped_for_json({m_value.as_string->characters(), m_value.as_string->length()});
            builder.append("\"");
        }
        break;
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
            builder.appendff("{}", m_value.as_double);
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
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @tparam Builder 
     * @return Builder::OutputType 
     */
    template <typename Builder>
    inline typename Builder::OutputType JsonValue::serialized() const
    {
        Builder builder;
        serialize(builder);
        return builder.build();
    }

} // namespace Mods

using Mods::JsonObject;
