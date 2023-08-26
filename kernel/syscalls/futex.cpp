/**
 * @file futex.cpp
 * @author Krisna Pranav
 * @brief futex
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/time.h>
#include <kernel/process.h>

namespace Kernel
{

    /**
     * @param userspace_address 
     * @return WaitQueue& 
     */
    WaitQueue& Process::futex_queue(Userspace<const i32*> userspace_address)
    {
        auto& queue = m_futex_queue.ensure(userspace_address.ptr());

        if (!queue)
            queue = make<WaitQueue>();
        
        return *queue;
    }

    int Process::sys$futex(Userspace<const Syscall::SC_futex_params*> user_params)
    {
        REQUIRE_PROMISE(thread);

        Syscall::SC_futex_params params;

        if (!copy_from_user(&params, user_params))
            return -EFAULT;
    }

} // namespace Kernel