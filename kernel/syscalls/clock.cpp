/**
 * @file clock.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief clock
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/time.h>
#include <kernel/process.h>
#include <kernel/time/timemanagement.h>

namespace Kernel
{
    int Process::sys$clock_gettime(clockid_t clock_id, Userspace<timespec*> user_ts)
    {
        REQUIRE_PROMISE(stdio);

        auto ts = TimeManagement::the().current_time(clock_id);

        if (ts.is_error())
            return ts.error();
        
        if (!copy_to_user(user_ts, &ts.value()))
            return -EFAULT;
        
        return 0;
    }

} // namespace Kernel