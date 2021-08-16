/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/memory/MemoryManager.h>
#include <kernel/memory/ProcessPagingScope.h>

namespace Kernel {

ProcessPagingScope::ProcessPagingScope(Process& process)
{
    VERIFY(Thread::current() != nullptr);
    m_previous_cr3 = read_cr3();
    MM.enter_process_paging_scope(process);
}

ProcessPagingScope::~ProcessPagingScope()
{
    InterruptDisabler disabler;
    Thread::current()->regs().cr3 = m_previous_cr3;
    write_cr3(m_previous_cr3);
}

}
