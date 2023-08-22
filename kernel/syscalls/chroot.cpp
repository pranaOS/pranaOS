/**
 * @file chroot.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief chroot
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/process.h>
#include <kernel/filesystem/custody.h>
#include <kernel/filesystem/virtualfilesystem.h>

namespace Kernel
{

    int Process::sys$chroot(Userspace<const char*> user_path, size_t path_length, int mount_flags)
    {
        if (!is_superuser())
            return -EPERM;
        
        REQUIRE_PROMISE(chroot);

        auto path = get_syscall_path_argument(user_path, path_length);

        if (path.is_error())
            return path.error();
        
        return 0;
    }
    
} // namespace Kernel