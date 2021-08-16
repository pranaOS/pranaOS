/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/filesystem/Custody.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$chroot(Userspace<const char*> user_path, size_t path_length, int mount_flags)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    if (!is_superuser())
        return EPERM;
    REQUIRE_PROMISE(chroot);
    auto path = get_syscall_path_argument(user_path, path_length);
    if (path.is_error())
        return path.error();
    auto directory_or_error = VirtualFileSystem::the().open_directory(path.value()->view(), current_directory());
    if (directory_or_error.is_error())
        return directory_or_error.error();
    auto directory = directory_or_error.value();
    m_root_directory_relative_to_global_root = directory;
    int chroot_mount_flags = mount_flags == -1 ? directory->mount_flags() : mount_flags;

    auto custody_or_error = Custody::try_create(nullptr, "", directory->inode(), chroot_mount_flags);
    if (custody_or_error.is_error())
        return custody_or_error.error();

    set_root_directory(custody_or_error.release_value());
    return 0;
}

}
