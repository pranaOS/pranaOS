/**
 * @file access.cpp
 * @author Krisna Pranav
 * @brief access
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/process.h>
#include <kernel/filesystem/virtualfilesystem.h>

namespace Kernel
{
    /// @breif: sys process access
    int Process::sys$access(Userspace<const char*> user_path, size_t path_length, int mode)
    {
        REQUIRE_PROMISE(rpath);
        auto path = get_syscall_path_argument(user_path, path_length)

        return VFS::the().access(path.value(), mode, current_directory());
    }

} // namespace Kernel