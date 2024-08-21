/**
 * @file property.h
 * @author Krisna Pranav
 * @brief property
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <mods/jsonval.h>

namespace Core 
{
    class Property 
    {
        MOD_MAKE_NONCOPYABLE(Property);

    public:
        /**
         * @param name 
         * @param getter 
         * @param setter 
         */
        Property(String name, Function<JsonValue()> getter, Function<bool(const JsonValue&)> setter = nullptr);

        /// @brief Destroy the Property object
        ~Property();

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool set(const JsonValue& value)
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
         * @return const String& 
         */
        const String& name() const 
        { 
            return m_name; 
        }

    private:
        String m_name;

        Function<JsonValue()> m_getter;
        Function<bool(const JsonValue&)> m_setter;
    }; // class Property
} // namespace Core