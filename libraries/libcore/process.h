/**
 * @file process.h
 * @author Krisna Pranav
 * @brief process
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/forward.h>

namespace Core 
{

    class Process 
    {
    public:
        /**
         * @param path 
         * @return pid_t 
         */
        static pid_t spawn(StringView path);
    }; // class Process 

} // namespace Core
