/**
 * @file chown.cpp
 * @author Krisna Pranav
 * @brief chown
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel 
{
    /**
     * @brief fchown sys
     * 
     */
    int Process::sys$fchown(int fd, uid_t uid, gid_t gid)
    {
        REQUIRE_PROMISE(chown);

        auto description = file_description(fd);

        if (!description)
            return -EBADF;

        return description->chown(uid, gid);
    }

    /**
     * @brief chown sys
     * 
     */
    int Process::sys$chown(Userspace<const Syscall::SC_chown_params*> user_params)
    {
        REQUIRE_PROMISE(chown);

        Syscall::SC_chown_params params;

        if (!copy_from_user(&params, user_params))
            return -EFAULT;

        auto path = get_syscall_path_argument(params.path);

        if (path.is_error())
            return path.error();

        return VFS::the().chown(path.value(), params.uid, params.gid, current_directory());
    }

} // namespace Kernel