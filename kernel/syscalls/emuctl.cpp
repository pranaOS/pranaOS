/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$emuctl()
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    return ENOSYS;
}

}
