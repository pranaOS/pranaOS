/**
 * @file ptrace.cpp
 * @author Krisna Pranav
 * @brief ptrace
 * @version 6.0
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

        if (params.tid == caller.pid().value())
            return KResult(-EINVAL);

        auto peer = Thread::from_tid(params.tid);

        if (!peer)
            return KResult(-ESRCH);

        if ((peer->process().uid() != caller.euid())
            || (peer->process().uid() != peer->process().euid())) 
            return KResult(-EACCES);

        auto& peer_process = peer->process();
        if (params.request == PT_ATTACH) {
            if (peer_process.tracer()) {
                return KResult(-EBUSY);
            }
            peer_process.start_tracing_from(caller.pid());
            ScopedSpinLock lock(peer->get_lock());
            if (peer->state() != Thread::State::Stopped) {
                peer->send_signal(SIGSTOP, &caller);
            }
            return KSuccess;
        }

        auto* tracer = peer_process.tracer();

        if (!tracer)
            return KResult(-EPERM);

        if (tracer->tracer_pid() != caller.pid())
            return KResult(-EBUSY);

        if (peer->state() == Thread::State::Running)
            return KResult(-EBUSY);

        switch (params.request) {
        case PT_CONTINUE:
            peer->send_signal(SIGCONT, &caller);
            break;

        case PT_DETACH:
            peer_process.stop_tracing();
            peer->send_signal(SIGCONT, &caller);
            break;

        case PT_SYSCALL:
            tracer->set_trace_syscalls(true);
            peer->send_signal(SIGCONT, &caller);

            break;

        case PT_GETREGS: {
            if (!tracer->has_regs())
                return KResult(-EINVAL);

            auto* regs = reinterpret_cast<PtraceRegisters*>(params.addr);

            if (!copy_to_user(regs, &tracer->regs()))
                return KResult(-EFAULT);

            break;
        }

        case PT_SETREGS: {
            if (!tracer->has_regs())
                return KResult(-EINVAL);

            PtraceRegisters regs;
            if (!copy_from_user(&regs, (const PtraceRegisters*)params.addr))
                return KResult(-EFAULT);

            auto& peer_saved_registers = peer->get_register_dump_from_stack();

            if ((peer_saved_registers.cs & 0x03) != 3)
                return KResult(-EFAULT);

            tracer->set_regs(regs);

            copy_ptrace_registers_into_kernel_registers(peer_saved_registers, regs);
            
            break;
        }

        case PT_PEEK: {
            Kernel::Syscall::SC_ptrace_peek_params peek_params;

            if (!copy_from_user(&peek_params, reinterpret_cast<Kernel::Syscall::SC_ptrace_peek_params*>(params.addr)))
                return -EFAULT;

            auto result = peer->process().peek_user_data((FlatPtr)peek_params.address);

            if (result.is_error())
                return -EFAULT;

            if (!copy_to_user(peek_params.out_data, &result.value()))
                return -EFAULT;
            break;
        }

        case PT_POKE: {
            Userspace<u32*> addr = reinterpret_cast<FlatPtr>(params.addr);
            return peer->process().poke_user_data(addr, params.data);
        }

        default:
            return -EINVAL;
        }

        return 0;
    }

    /**
     * @param ptrace_regs 
     * @param kernel_regs 
     */
    void copy_kernel_registers_into_ptrace_registers(PtraceRegisters& ptrace_regs, const RegisterState& kernel_regs)
    {
        ptrace_regs.eax = kernel_regs.eax,
        ptrace_regs.ecx = kernel_regs.ecx,
        ptrace_regs.edx = kernel_regs.edx,
        ptrace_regs.ebx = kernel_regs.ebx,
        ptrace_regs.esp = kernel_regs.userspace_esp,
        ptrace_regs.ebp = kernel_regs.ebp,
        ptrace_regs.esi = kernel_regs.esi,
        ptrace_regs.edi = kernel_regs.edi,
        ptrace_regs.eip = kernel_regs.eip,
        ptrace_regs.eflags = kernel_regs.eflags,
        ptrace_regs.cs = 0;
        ptrace_regs.ss = 0;
        ptrace_regs.ds = 0;
        ptrace_regs.es = 0;
        ptrace_regs.fs = 0;
        ptrace_regs.gs = 0;
    }

    /**
     * @param kernel_regs 
     * @param ptrace_regs 
     */
    void copy_ptrace_registers_into_kernel_registers(RegisterState& kernel_regs, const PtraceRegisters& ptrace_regs)
    {
        kernel_regs.eax = ptrace_regs.eax;
        kernel_regs.ecx = ptrace_regs.ecx;
        kernel_regs.edx = ptrace_regs.edx;
        kernel_regs.ebx = ptrace_regs.ebx;
        kernel_regs.esp = ptrace_regs.esp;
        kernel_regs.ebp = ptrace_regs.ebp;
        kernel_regs.esi = ptrace_regs.esi;
        kernel_regs.edi = ptrace_regs.edi;
        kernel_regs.eip = ptrace_regs.eip;
        kernel_regs.eflags = ptrace_regs.eflags;
    }

} // namespace Kernel