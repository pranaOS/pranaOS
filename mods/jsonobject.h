/**
 * @file jsonobject.h
 * @author Krisna Pranav
 * @brief json object
 * @version 6.0
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        JsonObject() {}
        ~JsonObject() {}

        JsonObject(const JsonObject& other)
            : m_order(other.m_order)
            , m_members(other.m_members)
        {}

        JsonObject(JsonObject&& other)
            : m_order(move(other.m_order))
            , m_members(move(other.m_members))
        {}

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
            if (this != other) {
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

    private:
        Vector<String> m_order;
        HashMap<String, JsonValue> m_members;
    }; 

} // namespace Mods