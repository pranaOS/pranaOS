/**
 * @file listener.h
 * @author Krisna Pranav
 * @brief listener
 * @version 6.0
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>

namespace Config 
{

    class Listener 
    {
    public:
        /**
         * @brief Destroy the Listener object
         * 
         */
        virtual ~Listener();
        
        static void for_each(Function<void(Listener&)>);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        virtual void config_string_did_change(String const& domain, String const& group, String const& key, String const& value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        virtual void config_i32_did_change(String const& domain, String const& group, String const& key, i32 value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        virtual void config_bool_did_change(String const& domain, String const& group, String const& key, bool value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         */
        virtual void config_key_was_removed(String const& domain, String const& group, String const& key);

    protected:
        /**
         * @brief Construct a new Listener object
         * 
         */
        Listener();
    }; // class Listener

} // namespace Config
