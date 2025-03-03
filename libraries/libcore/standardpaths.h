/**
 * @file standardpaths.h
 * @author Krisna Pranav
 * @brief standard paths
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>

namespace Core 
{

    class StandardPaths 
    {
    public:
        /**

         * @return String 
         */
        static String home_directory();
        static String desktop_directory();
        static String downloads_directory();
        static String tempfile_directory();
        static String config_directory();
    }; // class StandardPaths 

} // namespace Core
