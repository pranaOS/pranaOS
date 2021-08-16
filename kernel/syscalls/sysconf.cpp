/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Process.h>
#include <kernel/time/TimeManagement.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$sysconf(int name)
{
    VERIFY_NO_PROCESS_BIG_LOCK(this)
    switch (name) {
    case _SC_MONOTONIC_CLOCK:
        return 1;
    case _SC_NPROCESSORS_CONF:
    case _SC_NPROCESSORS_ONLN:
        return Processor::processor_count();
    case _SC_OPEN_MAX:
        return fds().max_open();
    case _SC_PAGESIZE:
        return PAGE_SIZE;
    case _SC_TTY_NAME_MAX:
        return TTY_NAME_MAX;
    case _SC_GETPW_R_SIZE_MAX:
        return 4096; 
    case _SC_CLK_TCK:
        return TimeManagement::the().ticks_per_second();
    default:
        return EINVAL;
    }
}

}
