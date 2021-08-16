/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/Custody.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::do_statvfs(String path, statvfs* buf)
{
    auto custody_or_error = VirtualFileSystem::the().resolve_path(path, current_directory(), nullptr, 0);
    if (custody_or_error.is_error())
        return custody_or_error.error();

    auto& custody = custody_or_error.value();
    auto& inode = custody->inode();
    auto& fs = inode.fs();

    statvfs kernelbuf = {};

    kernelbuf.f_bsize = static_cast<u64>(fs.block_size());
    kernelbuf.f_frsize = fs.fragment_size();
    kernelbuf.f_blocks = fs.total_block_count();
    kernelbuf.f_bfree = fs.free_block_count();

    kernelbuf.f_bavail = fs.free_block_count();

    kernelbuf.f_files = fs.total_inode_count();
    kernelbuf.f_ffree = fs.free_inode_count();
    kernelbuf.f_favail = fs.free_inode_count(); 

    kernelbuf.f_fsid = 0; 

    kernelbuf.f_namemax = 255;

    Custody* current_custody = custody;

    while (current_custody) {
        VirtualFileSystem::the().for_each_mount([&kernelbuf, &current_custody](auto& mount) {
            if (current_custody) {
                if (&current_custody->inode() == &mount.guest()) {
                    int mountflags = mount.flags();
                    int flags = 0;
                    if (mountflags & MS_RDONLY)
                        flags = flags | ST_RDONLY;
                    if (mountflags & MS_NOSUID)
                        flags = flags | ST_NOSUID;

                    kernelbuf.f_flag = flags;
                    current_custody = nullptr;
                }
            }
        });

        if (current_custody) {
            current_custody = current_custody->parent();
        }
    }

    if (!copy_to_user(buf, &kernelbuf))
        return EFAULT;

    return 0;
}

KResultOr<FlatPtr> Process::sys$statvfs(Userspace<const Syscall::SC_statvfs_params*> user_params)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(rpath);

    Syscall::SC_statvfs_params params;
    if (!copy_from_user(&params, user_params))
        return EFAULT;
    auto path = get_syscall_path_argument(params.path);
    if (path.is_error())
        return path.error();

    return do_statvfs(path.value()->view(), params.buf);
}

KResultOr<FlatPtr> Process::sys$fstatvfs(int fd, statvfs* buf)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(stdio);

    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;

    return do_statvfs(description->absolute_path(), buf);
}

}
