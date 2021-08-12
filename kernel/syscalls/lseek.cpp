/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$lseek(int fd, Userspace<off_t*> userspace_offset, int whence)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    off_t offset;
    if (!copy_from_user(&offset, userspace_offset))
        return EFAULT;
    auto seek_result = description->seek(offset, whence);
    if (seek_result.is_error())
        return seek_result.error();
    if (!copy_to_user(userspace_offset, &seek_result.value()))
        return EFAULT;
    return KSuccess;
}

}
