/**
 * @file noallocationguard.h
 * @author Krisna Pranav
 * @brief no allocation guard
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>
#include <mods/noncopyable.h>

#if defined(KERNEL)
#   include <kernel/arch/processor.h>
#else
#   include <libc/mallocdefs.h>
#endif

namespace Mods
{
    class NoAllocationGuard
    {

    }; // class NoAllocationGuard
} // namespace Mods