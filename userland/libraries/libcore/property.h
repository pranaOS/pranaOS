/**
 * @file property.h
 * @author Krisna Pranav
 * @brief property
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/jsonvalue.h>

namespace Core 
{

    class Property 
    {
        MOD_MAKE_NONCOPYABLE(Property);

    public:
        /**
         * @brief Construct a new Property object
         * 
         * @param name 
         * @param getter 
         * @param setter 
         */
        Property(String name, Function<JsonValue()> getter, Function<bool(JsonValue const&)> setter = nullptr);

        /**
         * @brief Destroy the Property object
         * 
         */
        ~Property() = default;

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool set(JsonValue const& value)
        {
            if (!m_setter)
                return false;

            return m_setter(value);
        }

        /**
         * @return JsonValue 
         */
        JsonValue get() const 
        { 
            return m_getter(); 
        }

        /**
         * @return String const& 
         */
        String const& name() const { return m_name; }

        /**
         * @return true 
         * @return false 
         */
        bool is_readonly() const { return !m_setter; }

    private:
        String m_name;
        Function<JsonValue()> m_getter;
        Function<bool(JsonValue const&)> m_setter;
    }; // class Property

} // namespace Core