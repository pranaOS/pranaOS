/**
 * @file jsonvalue.cpp
 * @author Krisna Pranav
 * @brief Json Value
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/jsonarray.h>
#include <mods/jsonobject.h>
#include <mods/jsonvalue.h>
#include <mods/stringview.h>

#ifndef KERNEL
#   include <mods/jsonparser.h>
#endif

namespace Mods
{
    /**
     * @brief Construct a new Json Value:: Json Value object
     * 
     * @param type 
     */
    JsonValue::JsonValue(Type type)
        : m_type(type)
    {}

    /**
     * @brief Construct a new Json Value:: Json Value object
     * 
     * @param other 
     */
    JsonValue::JsonValue(JsonValue const& other)
    {
        copy_from(other);
    }   

    /**
     * @param other 
     * @return JsonValue& 
     */
    JsonValue& JsonValue::operator=(JsonValue const& other)
    {
        if (this != other) {
            clear();
            copy_from(other);
        }

        return *this;
    }
}