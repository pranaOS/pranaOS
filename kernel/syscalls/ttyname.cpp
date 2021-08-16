/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>
#include <kernel/tty/MasterPTY.h>
#include <kernel/tty/TTY.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$ttyname(int fd, Userspace<char*> buffer, size_t size)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(tty);
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    if (!description->is_tty())
        return ENOTTY;
    auto tty_name = description->tty()->tty_name();
    if (size < tty_name.length() + 1)
        return ERANGE;
    if (!copy_to_user(buffer, tty_name.characters(), tty_name.length() + 1))
        return EFAULT;
    return 0;
}

KResultOr<FlatPtr> Process::sys$ptsname(int fd, Userspace<char*> buffer, size_t size)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(tty);
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    auto* master_pty = description->master_pty();
    if (!master_pty)
        return ENOTTY;
    auto pts_name = master_pty->pts_name();
    if (size < pts_name.length() + 1)
        return ERANGE;
    if (!copy_to_user(buffer, pts_name.characters(), pts_name.length() + 1))
        return EFAULT;
    return 0;
}

}
