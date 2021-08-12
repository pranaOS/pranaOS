/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Process.h>
#include <libc/sys/prctl_numbers.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$prctl(int option, FlatPtr arg1, [[maybe_unused]] FlatPtr arg2)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    switch (option) {
    case PR_GET_DUMPABLE:
        return is_dumpable();
    case PR_SET_DUMPABLE:
        set_dumpable(arg1);
        return 0;
    default:
        return EINVAL;
    }
    return 0;
}

}
