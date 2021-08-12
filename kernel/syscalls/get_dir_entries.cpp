/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$get_dir_entries(int fd, Userspace<void*> user_buffer, size_t user_size)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    REQUIRE_PROMISE(stdio);
    if (user_size > NumericLimits<ssize_t>::max())
        return EINVAL;
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    auto buffer = UserOrKernelBuffer::for_user_buffer(user_buffer, static_cast<size_t>(user_size));
    if (!buffer.has_value())
        return EFAULT;
    auto result = description->get_dir_entries(buffer.value(), user_size);
    if (result.is_error())
        return result.error();
    else
        return result.release_value();
}

}
