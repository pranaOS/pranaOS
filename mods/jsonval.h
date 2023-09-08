/**
 * @file jsonval.h
 * @author Krisna Pranav
 * @brief json value
 * @version 6.0
 * @date 2023-09-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>
#include <mods/ipv4address.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <mods/string_builder.h>

namespace Mods 
{
    class JsonValue 
    {
    public:
        enum class Type 
        {
            Null,
            Int32,
            UnsignedInt32,
            Int64,
            UnsignedInt64,
    #if !defined(KERNEL)
            Double,
    #endif
            Bool,
            String,
            Array,
            Object,
        };

        static Optional<JsonValue> from_string(const StringView&);

        /// @brief Construct a new Json Value object
        explicit JsonValue(Type = Type::Null);

        /// @brief Destroy the Json Value object
        ~JsonValue() 
        { 
            clear(); 
        }

        JsonValue(const JsonValue&);
        JsonValue(JsonValue&&);

        JsonValue& operator=(const JsonValue&);
        JsonValue& operator=(JsonValue&&);

        JsonValue(int);
        JsonValue(unsigned);
        JsonValue(long);
        JsonValue(long unsigned);
        JsonValue(long long);
        JsonValue(long long unsigned);

    #if !defined(KERNEL)
        JsonValue(double);
    #endif

        JsonValue(bool);
        JsonValue(const char*);
        JsonValue(const String&);
        JsonValue(const IPv4Address&);
        JsonValue(const JsonArray&);
        JsonValue(const JsonObject&);

        JsonValue(JsonArray&&);
        JsonValue(JsonObject&&);

        JsonValue& operator=(JsonArray&&) = delete;
        JsonValue& operator=(JsonObject&&) = delete;

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
         * @param alternative 
         * @return String 
         */
        String as_string_or(const String& alternative)
        {
            if (is_string())
                return as_string();

            return alternative;
        }

        /**
         * @return String 
         */
        String to_string() const
        {
            if (is_string())
                return as_string();

            return serialized<StringBuilder>();
        }

        /**
         * @return Optional<IPv4Address> 
         */
        Optional<IPv4Address> to_ipv4_address() const
        {
            if (!is_string())
                return {};

            return IPv4Address::from_string(as_string());
        }

        /**
         * @param default_value 
         * @return int 
         */
        int to_int(int default_value = 0) const 
        { 
            return to_i32(default_value); 
        }

        /**
         * @param default_value 
         * @return i32 
         */
        i32 to_i32(i32 default_value = 0) const 
        { 
            return to_number<i32>(default_value); 
        }

        /**
         * @param default_value 
         * @return unsigned 
         */
        unsigned to_uint(unsigned default_value = 0) const 
        { 
            return to_u32(default_value); 
        }

        /**
         * @param default_value 
         * @return u32 
         */
        u32 to_u32(u32 default_value = 0) const 
        { 
            return to_number<u32>(default_value); 
        }
    
        /**
         * @param default_value 
         * @return true 
         * @return false 
         */
        bool to_bool(bool default_value = false) const
        {
            if (!is_bool())
                return default_value;

            return as_bool();
        }

        /**
         * @return i32 
         */
        i32 as_i32() const
        {
            ASSERT(is_i32());
            return m_value.as_i32;
        }

        /**
         * @return u32 
         */
        u32 as_u32() const
        {
            ASSERT(is_u32());
            return m_value.as_u32;
        }

        /**
         * @return i64 
         */
        i64 as_i64() const
        {
            ASSERT(is_i64());
            return m_value.as_i64;
        }

        /**
         * @return u64 
         */
        u64 as_u64() const
        {
            ASSERT(is_u64());
            return m_value.as_u64;
        }

        int as_bool() const
        {
            ASSERT(is_bool());
            return m_value.as_bool;
        }

        String as_string() const
        {
            ASSERT(is_string());
            return *m_value.as_string;
        }

        const JsonObject& as_object() const
        {
            ASSERT(is_object());
            return *m_value.as_object;
        }

        /**
         * @return const JsonArray& 
         */
        const JsonArray& as_array() const
        {
            ASSERT(is_array());
            return *m_value.as_array;
        }

    #if !defined(KERNEL)
        double as_double() const
        {
            ASSERT(is_double());
            return m_value.as_double;
        }
    #endif

        /**
         * @return Type 
         */
        Type type() const
        {
            return m_type;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const 
        { 
            return m_type == Type::Null; 
        }

        bool is_bool() const 
        { 
            return m_type == Type::Bool; 
        }
        
        bool is_string() const 
        { 
            return m_type == Type::String; 
        }

        bool is_i32() const 
        { 
            return m_type == Type::Int32; 
        }

        bool is_u32() const 
        { 
            return m_type == Type::UnsignedInt32; 
        }

        bool is_i64() const 
        { 
            return m_type == Type::Int64; 
        }

        bool is_u64() const 
        { 
            return m_type == Type::UnsignedInt64; 
        }
        
    #if !defined(KERNEL)
        bool is_double() const
        {
            return m_type == Type::Double;
        }
    #endif

        /**
         * @return true 
         * @return false 
         */
        bool is_array() const
        {
            return m_type == Type::Array;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_object() const 
        { 
            return m_type == Type::Object; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_number() const
        {
            switch (m_type) 
            {
            case Type::Int32:
            case Type::UnsignedInt32:
            case Type::Int64:
            case Type::UnsignedInt64:
    #if !defined(KERNEL)
            case Type::Double:
    #endif
                return true;
            default:
                return false;
            }
        }

        /**
         * @tparam T 
         * @param default_value 
         * @return T 
         */
        template<typename T>
        T to_number(T default_value = 0) const
        {
    #if !defined(KERNEL)
            if (is_double())
                return (T)as_double();
    #endif
            if (type() == Type::Int32)
                return (T)as_i32();
            if (type() == Type::UnsignedInt32)
                return (T)as_u32();
            if (type() == Type::Int64)
                return (T)as_i64();
            if (type() == Type::UnsignedInt64)
                return (T)as_u64();
            return default_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool equals(const JsonValue& other) const;

    private:
        void clear();
        
        void copy_from(const JsonValue&);

        Type m_type { Type::Null };

        union 
        {
            StringImpl* as_string { nullptr };
            JsonArray* as_array;
            JsonObject* as_object;
    #if !defined(KERNEL)
            double as_double;
    #endif
            i32 as_i32;
            u32 as_u32;
            i64 as_i64;
            u64 as_u64;
            bool as_bool;
        } m_value;
    }; // class JsonValue

    /**
     * @tparam -
     */
    template<>
    struct Formatter<JsonValue> : Formatter<StringView> 
    {
        /**
         * @param params 
         * @param builder 
         * @param value 
         */
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const JsonValue& value)
        {
            Formatter<StringView>::format(params, builder, value.to_string());
        }
    }; // template

} // namespace Mods

using Mods::JsonValue;
