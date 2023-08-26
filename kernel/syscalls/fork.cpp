/**
 * @file fork.cpp
 * @author Krisna Pranav
 * @brief fork
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
     * @brief pid_t fork process
     * 
     */
    pid_t Process::sys$fork(RegisterState& regs)
    {
        REQUIRE_PROMISE(proc);

        RefPtr<Thread> child_first_thread;

        auto child = adopt(*new Process(child_first_thread, m_name, m_uid, m_thread));

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

        SharedBuffer::share_all_shared_buffer(*this, *child);

        {
            ScopedSpinLock lock(m_lock);
        }        
    }
}