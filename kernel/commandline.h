/**
 * @file commandline.h
 * @author Krisna Pranav
 * @brief commandline
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashmap.h>
#include <mods/optional.h>
#include <mods/string.h>

namespace Kernel 
{

    class CommandLine 
    {
        MOD_MAKE_ETERNAL;

    public:
        /**
         * @param cmd_line 
         */
        static void early_initialize(const char* cmd_line);

        /// @brief initialize
        static void initialize();

        /**
         * @return const String& 
         */
        const String& string() const { 
            return m_string; 
        }

        /**
         * @param key 
         * @return Optional<String> 
         */
        Optional<String> lookup(const String& key) const;

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool contains(const String& key) const;

    private:
        /**
         * @brief Construct a new Command Line object
         * 
         */
        CommandLine(const String&);

        /// @brief m_string
        String m_string;

        /// @brief m_params
        HashMap<String, String> m_params;
    };

    /**
     * @return const CommandLine& 
     */
    const CommandLine& kernel_command_line();

}
