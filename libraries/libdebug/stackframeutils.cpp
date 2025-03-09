/**
 * @file stackframeutils.cpp
 * @author Krisna Pranav
 * @brief Stack Frame Utils
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "stackframeutils.h"

namespace Debug::StackFrameUtils 
{
    
    /**
     * @brief Get the info object
     * 
     * @param inspector 
     * @param current_ebp 
     * @return Optional<StackFrameInfo> 
     */
    Optional<StackFrameInfo> get_info(ProcessInspector const& inspector, FlatPtr current_ebp)
    {
        auto return_address = inspector.peek(current_ebp + sizeof(FlatPtr));
        auto next_ebp = inspector.peek(current_ebp);

        if (!return_address.has_value() || !next_ebp.has_value())
            return {};

        StackFrameInfo info = { return_address.value(), next_ebp.value() };
        
        return info;
    }

} // namespace Debug::StackFrameUtils 
