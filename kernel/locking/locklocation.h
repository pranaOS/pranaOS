/**
 * @file locklocation.h
 * @author Krisna Pranav
 * @brief Lock Location
 * @version 6.0
 * @date 2024-09-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextra.h>
#include <kernel/debug.h>
#if LOCK_DEBUG
#    include <mods/sourcelocation.h>
#endif

namespace Kernel 
{

    #if LOCK_DEBUG
    using LockLocation = SourceLocation;
    #else
    struct LockLocation 
    {
        static constexpr LockLocation current() 
        { 
            return {}; 
        }

    private:
        constexpr LockLocation() = default;
    }; // struct LockLocation
    #endif

} // namespace Kernel