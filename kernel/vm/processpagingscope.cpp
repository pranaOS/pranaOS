/**
 * @file processpagingscope.cpp
 * @author Krisna Pranav
 * @brief Process Paging Scope
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "processpagingscope.h"
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/processpagingscope.h>

namespace Kernel
{
    /**
     * @param process 
     */
    ProcessPagingScope::ProcessPagingScope(Process& process)
    {
        ASSERT(Thread::current() != nullptr);
    }

    ProcessPagingScope::~ProcessPagingScope()
    {
        InterruptDisabler disabler;
        Thread::current()->tss().cr3 = m_previous_cr3;
    }

} // namespace Kernel