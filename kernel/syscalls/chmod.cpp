/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$chmod(Userspace<const char*> user_path, size_t path_length, mode_t mode)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    REQUIRE_PROMISE(fattr);
    auto path = get_syscall_path_argument(user_path, path_length);
    if (path.is_error())
        return path.error();
    return VirtualFileSystem::the().chmod(path.value()->view(), mode, current_directory());
}

KResultOr<FlatPtr> Process::sys$fchmod(int fd, mode_t mode)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    REQUIRE_PROMISE(fattr);
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    return description->chmod(mode);
}

}

