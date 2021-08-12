/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$ftruncate(int fd, Userspace<off_t*> userspace_length)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    off_t length;
    if (!copy_from_user(&length, userspace_length))
        return EFAULT;
    if (length < 0)
        return EINVAL;
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    if (!description->is_writable())
        return EBADF;
    return description->truncate(static_cast<u64>(length));
}

}
