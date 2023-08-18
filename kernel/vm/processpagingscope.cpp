/**
 * @file processpagingscope.cpp
 * @author Krisna Pranav
 * @brief Process Paging Scope
 * @version 6.0
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
        m_previous_cr3 = read_cr3();
        MM.enter_process_paging_scope(process);
    }

    /// @brief Destroy the Process Paging Scope:: Process Paging Scope object
    ProcessPagingScope::~ProcessPagingScope()
    {
        InterruptDisabler disabler;
        Thread::current()->tss().cr3 = m_previous_cr3;
        write_cr3(m_previous_cr3);
    }

} // namespace Kernel