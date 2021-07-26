/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/ProcessPagingScope.h>

namespace Kernel {

ProcessPagingScope::ProcessPagingScope(Process& process)
{
    VERIFY(Thread::current() != nullptr);
    m_previous_cr3 = read_cr3();
    MM.enter_process_paging_scoped(process);
}

ProcessPagingScope::~ProcessPagingScope()
{
    InterruptDisabler disabler
    Thread::current()->regs().cr3 = m_previous_cr3;
    write_cr3(m_previous_cr3);
}

}
