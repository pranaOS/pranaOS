/**
 * @file jsonarray.h
 * @author Krisna Pranav
 * @brief json array
 * @version 6.0
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        JsonArray() {}
        ~JsonArray() {}

        JsonArray(const JsonArray& other)
            : m_values(other.m_values)
        {}

        JsonArray(JsonArray&& other)
            : m_values(move(other.m_values))
        {}

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

    private:
        Vector<JsonValue> m_values;

    }; // class JsonArray
}