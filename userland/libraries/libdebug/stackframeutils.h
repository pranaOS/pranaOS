/**
 * @file stackframeutils.h
 * @author Krisna Pranav
 * @brief stack frame utils
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/types.h>
#include "libdebug/debugsession.h"

namespace Debug::StackFrameUtils 
{

    struct StackFrameInfo {
        FlatPtr return_address;
        FlatPtr next_ebp;
    }; // struct StackFrameInfo

    /**
     * @brief Get the info object
     * 
     * @param current_ebp 
     * @return Optional<StackFrameInfo> 
     */
    Optional<StackFrameInfo> get_info(ProcessInspector const&, FlatPtr current_ebp);

} // namespace Debug::StackFrameUtils 
