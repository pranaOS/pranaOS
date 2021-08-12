/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$umask(mode_t mask)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    auto old_mask = m_umask;
    ProtectedDataMutationScope scope { *this };
    m_umask = mask & 0777;
    return old_mask;
}

}
