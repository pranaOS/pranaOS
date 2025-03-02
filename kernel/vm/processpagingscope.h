/**
 * @file processpagingscope.h
 * @author Krisna Pranav
 * @brief Process Paging Scope
 * @version 6.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <kernel/forward.h>

namespace Kernel 
{

    class ProcessPagingScope 
    {
    public:
        /// @brief Destroy the Process Paging Scope object
        explicit ProcessPagingScope(Process&);
        ~ProcessPagingScope();

    private:
        u32 m_previous_cr3 { 0 };
    };

} // namespace Kernel