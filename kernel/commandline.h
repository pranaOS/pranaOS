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
    private:
        CommandLine(const String&);
        String m_string;
        HashMap<String, String> m_params;

    public:

        /**
         * @param cmd_line 
         */
        static void early_initialize(const char* cmd_line);
        
        /// initialize
        static void initialize();

        /**
         * @return const String& 
         */
        const String& string() const {
            return m_string;
        }

        bool contains(const String& key) const;

    };
    
    /**
     * @return const CommandLine& 
     */
    const CommandLine& kernel_command_line_init();
    
} // namespace Kernel
