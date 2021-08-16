/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/KSyms.h>
#include <kernel/PerformanceManager.h>
#include <kernel/Process.h>
#include <kernel/Thread.h>

namespace Kernel {

void Process::sys$exit(int status)
{

    if (Thread::current()->previous_mode() == Thread::PreviousMode::UserMode) {
        VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    }

    {
        ProtectedDataMutationScope scope { *this };
        m_termination_status = status;
        m_termination_signal = 0;
    }

    auto* current_thread = Thread::current();
    current_thread->set_profiling_suppressed();
    PerformanceManager::add_thread_exit_event(*current_thread);

    die();
    current_thread->die_if_needed();
    VERIFY_NOT_REACHED();
}

}
