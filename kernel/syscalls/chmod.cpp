/**
 * @file chmod.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief chmod
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/filesystem/filedescription.h>
#include <kernel/filesystem/virtualfilesystem.h>
#include <kernel/process.h>

namespace Kernel 
{
    /**
     * @brief chmod sys
     * 
     */
    int Process::sys$chmod(Userspace<const char*> user_path, size_t path_length, mode_t mode)
    {
        REQUIRE_PROMISE(fattr);

        auto path = get_syscall_path_argument(user_path, path_length);

        if (path.is_error())
            return path.error();

        return VFS::the().chmod(path.value(), mode, current_directory());
    }

    /**
     * @brief sys fchmod
     * 
     */
    int Process::sys$fchmod(int fd, mode_t mode)
    {
        REQUIRE_PROMISE(fattr);

        auto description = file_description(fd);
        
        if (!description)
            return -EBADF;

        return description->chmod(mode);
    }

} // namespace Kernel