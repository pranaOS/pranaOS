/**
 * @file disown.cpp
 * @author Krisna Pranav
 * @brief disown
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>

namespace Kernel 
{
    /**
     * @brief disown sys
     * 
     */
    int Process::sys$disown(ProcessID pid)
    {
        
        REQUIRE_PROMISE(proc);
        auto process = Process::from_pid(pid);

        if (!process)
            return -ESRCH;

        if (process->ppid() != this->pid())
            return -ECHILD;

        process->m_ppid = 0;
        process->disowned_by_waiter(*this);

        return 0;
    }

} // namespace Kernel