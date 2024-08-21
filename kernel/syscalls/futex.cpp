/**
 * @file futex.cpp
 * @author Krisna Pranav
 * @brief futex
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        auto& queue = m_futex_queues.ensure(userspace_address.ptr());

        if (!queue)
            queue = make<WaitQueue>();

        return *queue;
    }

    /**
     * @brief futex sys process
     * 
     */
    int Process::sys$futex(Userspace<const Syscall::SC_futex_params*> user_params)
    {
        REQUIRE_PROMISE(thread);

        Syscall::SC_futex_params params;

        if (!copy_from_user(&params, user_params))
            return -EFAULT;

        switch (params.futex_op) {

        case FUTEX_WAIT: {
            i32 user_value;

            if (!copy_from_user(&user_value, params.userspace_address))
                return -EFAULT;

            if (user_value != params.val)
                return -EAGAIN;

            Thread::BlockTimeout timeout;

            if (params.timeout) {
                timespec ts_abstimeout { 0, 0 };

                if (!copy_from_user(&ts_abstimeout, params.timeout))
                    return -EFAULT;

                timeout = Thread::BlockTimeout(true, &ts_abstimeout);
            }

            WaitQueue& wait_queue = futex_queue((FlatPtr)params.userspace_address);

            Thread::BlockResult result = wait_queue.wait_on(timeout, "Futex");

            if (result == Thread::BlockResult::InterruptedByTimeout) {
                return -ETIMEDOUT;
            }

            break;
        }
        case FUTEX_WAKE:
            if (params.val == 0)
                return 0;
                
            if (params.val == 1) {
                futex_queue((FlatPtr)params.userspace_address).wake_one();
            } else {
                futex_queue((FlatPtr)params.userspace_address).wake_n(params.val);
            }
            break;
        }

        return 0;
    }

} // namespace Kernel
