/**
 * @file ptrace.cpp
 * @author Krisna Pranav
 * @brief ptrace
 * @version 6.0
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/ptrace.h>
#include <kernel/thread.h>
#include <kernel/threadtracer.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/processpagingscope.h>

namespace Ptrace
{
    /**
     * @param params 
     * @param caller 
     * @return KResultOr<u32> 
     */
    KResultOr<u32> handle_syscall(const Kernel::Syscall::SC_ptrace_params& params, Process& caller)
    {
        ScopedSpinLock scheduler_lock(g_scheduler_lock);

        if (params.request == PT_TRACE_ME) {
            if (Process::current()->tracer())
                return KResult(-EBUSY);
            
            caller.set_wait_for_tracer_at_next_execve(true);
            return KSuccess;
        }

        if (params.tid == caler.pid().value())
            return KResult(-EINVAL);
        
        auto peer = Thread::from_tid(params.tid);

        if (!peer)
            return KResult(-ESRCH);
        
        if ((peer->process().uid() != caller.euid()))
            || (peer->process().uid() != peer->process().euid())
            return KResult(-EACCES);

        auto &peer_process = peer->process();

        if (params.request == PT_ATTACH) {
            if (peer_process.tracer()) {
                return KResult(-EBUSY);
            }
            peer_process.start_tracing_from(caller.pid());

            if (peer->state() != Thread::State::Stopped) {
                peer->send_signal(SIGSTOP, &caller);
            }

            return KSuccess;
        }
    }
} // namespace Kernel