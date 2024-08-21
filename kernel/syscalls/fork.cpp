/**
 * @file fork.cpp
 * @author Krisna Pranav
 * @brief fork
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/sharedbuffer.h>
#include <kernel/filesystem/custody.h>
#include <kernel/filesystem/filedescription.h>
#include <kernel/vm/region.h>


namespace Kernel 
{

    /**
     * @brief pid_t fork sys process
     * 
     */
    pid_t Process::sys$fork(RegisterState& regs)
    {
        REQUIRE_PROMISE(proc);

        RefPtr<Thread> child_first_thread;

        auto child = adopt(*new Process(child_first_thread, m_name, m_uid, m_gid, m_pid, m_is_kernel_process, m_cwd, m_executable, m_tty, this));

        if (!child_first_thread)
            return -ENOMEM;

        child->m_root_directory = m_root_directory;
        child->m_root_directory_relative_to_global_root = m_root_directory_relative_to_global_root;
        child->m_promises = m_promises;
        child->m_execpromises = m_execpromises;
        child->m_veil_state = m_veil_state;
        child->m_unveiled_paths = m_unveiled_paths;
        child->m_fds = m_fds;
        child->m_sid = m_sid;
        child->m_pg = m_pg;
        child->m_umask = m_umask;

    #ifdef FORK_DEBUG
        dbg() << "fork: child=" << child;
    #endif

        child->m_extra_gids = m_extra_gids;

        auto& child_tss = child_first_thread->m_tss;

        child_tss.eax = 0; 
        child_tss.ebx = regs.ebx;
        child_tss.ecx = regs.ecx;
        child_tss.edx = regs.edx;
        child_tss.ebp = regs.ebp;
        child_tss.esp = regs.userspace_esp;
        child_tss.esi = regs.esi;
        child_tss.edi = regs.edi;
        child_tss.eflags = regs.eflags;
        child_tss.eip = regs.eip;
        child_tss.cs = regs.cs;
        child_tss.ds = regs.ds;
        child_tss.es = regs.es;
        child_tss.fs = regs.fs;
        child_tss.gs = regs.gs;
        child_tss.ss = regs.userspace_ss;

    #ifdef FORK_DEBUG
        dbg() << "fork: child will begin executing at " << String::format("%w", child_tss.cs) << ":" << String::format("%x", child_tss.eip) << " with stack " << String::format("%w", child_tss.ss) << ":" << String::format("%x", child_tss.esp) << ", kstack " << String::format("%w", child_tss.ss0) << ":" << String::format("%x", child_tss.esp0);
    #endif

        SharedBuffer::share_all_shared_buffers(*this, *child);

        {
            ScopedSpinLock lock(m_lock);

            for (auto& region : m_regions) {
    #ifdef FORK_DEBUG
                dbg() << "fork: cloning Region{" << &region << "} '" << region.name() << "' @ " << region.vaddr();
    #endif
                auto& child_region = child->add_region(region.clone());

                child_region.map(child->page_directory());

                if (&region == m_master_tls_region.unsafe_ptr())
                    child->m_master_tls_region = child_region;
            }

            ScopedSpinLock processes_lock(g_processes_lock);

            g_processes->prepend(child);

            child->ref(); 
        }

        ScopedSpinLock lock(g_scheduler_lock);

        child_first_thread->set_affinity(Thread::current()->affinity());
        child_first_thread->set_state(Thread::State::Runnable);

        return child->pid().value();
    }

} // namespace Kernel
