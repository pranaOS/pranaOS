/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


 includes
#include <base/StringView.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$rename(Userspace<const Syscall::SC_rename_params*> user_params)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(cpath);
    Syscall::SC_rename_params params;
    if (!copy_from_user(&params, user_params))
        return EFAULT;
    auto old_path = get_syscall_path_argument(params.old_path);
    if (old_path.is_error())
        return old_path.error();
    auto new_path = get_syscall_path_argument(params.new_path);
    if (new_path.is_error())
        return new_path.error();
    return VirtualFileSystem::the().rename(old_path.value()->view(), new_path.value()->view(), current_directory());
}

}
