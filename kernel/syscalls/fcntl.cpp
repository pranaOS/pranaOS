/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Debug.h>
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$fcntl(int fd, int cmd, u32 arg)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this);
    REQUIRE_PROMISE(stdio);
    dbgln_if(IO_DEBUG, "sys$fcntl: fd={}, cmd={}, arg={}", fd, cmd, arg);
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;

    switch (cmd) {
    case F_DUPFD: {
        int arg_fd = (int)arg;
        if (arg_fd < 0)
            return EINVAL;
        auto new_fd_or_error = fds().allocate(arg_fd);
        if (new_fd_or_error.is_error())
            return new_fd_or_error.error();
        auto new_fd = new_fd_or_error.release_value();
        m_fds[new_fd.fd].set(*description);
        return new_fd.fd;
    }
    case F_GETFD:
        return m_fds[fd].flags();
    case F_SETFD:
        m_fds[fd].set_flags(arg);
        break;
    case F_GETFL:
        return description->file_flags();
    case F_SETFL:
        description->set_file_flags(arg);
        break;
    case F_ISTTY:
        return description->is_tty();
    case F_GETLK:
        return description->get_flock(Userspace<flock*>(arg));
    case F_SETLK:
        return description->apply_flock(*Process::current(), Userspace<const flock*>(arg));
    default:
        return EINVAL;
    }
    return 0;
}

}
