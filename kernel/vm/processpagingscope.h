/**
 * @file processpagingscope.h
 * @author Krisna Pranav
 * @brief Process Paging Scope
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        ~ProcessPagingScope();

    private:
        u32 m_previos_cr2 { 0 };
    }; 
} // namespace Kernel