/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$dup2(int old_fd, int new_fd)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    REQUIRE_PROMISE(stdio);
    auto description = fds().file_description(old_fd);
    if (!description)
        return EBADF;
    if (old_fd == new_fd)
        return new_fd;
    if (new_fd < 0 || static_cast<size_t>(new_fd) >= fds().max_open())
        return EINVAL;
    if (!m_fds.m_fds_metadatas[new_fd].is_allocated())
        m_fds.m_fds_metadatas[new_fd].allocate();
    m_fds[new_fd].set(*description);
    return new_fd;
}

}
