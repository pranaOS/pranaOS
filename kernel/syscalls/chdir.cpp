/**
 * @file chdir.cpp
 * @author Krisna Pranav
 * @brief chdir
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/filesystem/custody.h>
#include <kernel/filesystem/filedescription.h>
#include <kernel/filesystem/virtualfilesystem.h>
#include <kernel/process.h>

namespace Kernel
{
    /**
     * @brief chdir sys
     * 
     */
    int Process::sys$chdir(Userspace<const char*> user_path, size_t path_length)
    {
        REQUIRE_PROMISE(rpath);

        auto path = get_syscall_path_argument(user_path, path_length);

        if (path.is_error()) 
            return path.error();
        
        auto directory_or_error = VFS::the().open_directory(path.value(), current_directory());

        if (directory_or_error.is_error())
            return directory_or_error.error();
        
        return 0;
    }

    /**
     * @brief getcwd sys
     * 
     */
    int Process::sys$getcwd(Userspace<char*> buffer, ssize_t size)
    {
        REQUIRE_PROMISE(rpath);

        if (size < 0)
            return -EINVAL;
        
        auto path = current_directory().absolute_path();

        if ((size_t)size < path.length() + 1)
            return -ERANGE;
        
        if(!copy_to_user(buffer, path.characters(), path.length() + 1))
            return -EFAULT;

        return 0;
    }
} // namespace Kernel