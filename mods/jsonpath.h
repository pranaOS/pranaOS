/**
 * @file jsonpath.h
 * @author Krisna Pranav
 * @brief Json Path
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Mods
{
    class JsonPathElement
    {
    public:
        enum class Kind
        {
            Key,
            Index,
            AnyIndex,
            AnyKey,
        }; // enum class Kind

        /**
         * @brief Construct a new Json Path Element object
         * 
         * @param index 
         */
        JsonPathElement(size_t index)
            : m_kind(Kind::Index)
            , m_index(index)
        {}

        /**
         * @brief Construct a new Json Path Element object
         * 
         * @param key 
         */
        JsonPathElement(StringView key)
            : m_kind(Kind::Key)
            , m_key(key)
        { }
    private:
        Kind m_kind;
        String m_key;
        size_t m_index { 0 };

        /**
         * @brief Construct a new Json Path Element object
         * 
         * @param kind 
         */
        JsonPathElement(Kind kind) 
            : m_kind(kind)
        {}
    }; // class JsonPathElement

    class JsonPath : public Vector<JsonPathElement>
    {
    public:
        /**
         * @return JsonValue 
         */
        JsonValue resolve(JsonValue const&) const;

        String to_string() const;
    }; // class JsonPath : public Vector<JsonPathElement
} // namespace Mods