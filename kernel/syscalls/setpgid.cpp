/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/Process.h>
#include <kernel/tty/TTY.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$getsid(pid_t pid)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(proc);
    if (pid == 0)
        return sid().value();
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    if (sid() != process->sid())
        return EPERM;
    return process->sid().value();
}

KResultOr<FlatPtr> Process::sys$setsid()
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(proc);
    InterruptDisabler disabler;
    bool found_process_with_same_pgid_as_my_pid = false;
    Process::for_each_in_pgrp(pid().value(), [&](auto&) {
        found_process_with_same_pgid_as_my_pid = true;
        return IterationDecision::Break;
    });
    if (found_process_with_same_pgid_as_my_pid)
        return EPERM;

    m_pg = ProcessGroup::create(ProcessGroupID(pid().value()));
    m_tty = nullptr;
    ProtectedDataMutationScope scope { *this };
    m_sid = pid().value();
    return sid().value();
}

KResultOr<FlatPtr> Process::sys$getpgid(pid_t pid)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(proc);
    if (pid == 0)
        return pgid().value();
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    return process->pgid().value();
}

KResultOr<FlatPtr> Process::sys$getpgrp()
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    return pgid().value();
}

SessionID Process::get_sid_from_pgid(ProcessGroupID pgid)
{

    SessionID sid { -1 };
    Process::for_each_in_pgrp(pgid, [&](auto& process) {
        sid = process.sid();
        return IterationDecision::Break;
    });

    return sid;
}

KResultOr<FlatPtr> Process::sys$setpgid(pid_t specified_pid, pid_t specified_pgid)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(proc);
    ProcessID pid = specified_pid ? ProcessID(specified_pid) : this->pid();
    if (specified_pgid < 0) {

        return EINVAL;
    }
    auto process = Process::from_pid(pid);
    if (!process)
        return ESRCH;
    if (process != this && process->ppid() != this->pid()) {

        return ESRCH;
    }
    if (process->is_session_leader()) {

        return EPERM;
    }
    if (process->ppid() == this->pid() && process->sid() != sid()) {

        return EPERM;
    }

    ProcessGroupID new_pgid = specified_pgid ? ProcessGroupID(specified_pgid) : process->pid().value();
    SessionID current_sid = sid();
    SessionID new_sid = get_sid_from_pgid(new_pgid);
    if (new_sid != -1 && current_sid != new_sid) {
        // Can't move a process between sessions.
        return EPERM;
    }
    if (new_sid == -1 && new_pgid != process->pid().value()) {
        // The value of the pgid argument is valid, but is not
        // the calling pid, and is not an existing process group.
        return EPERM;
    }

    process->m_pg = ProcessGroup::find_or_create(new_pgid);
    if (!process->m_pg) {
        return ENOMEM;
    }
    return 0;
}

}
