/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>
#include <libc/sys/ioctl_numbers.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$ioctl(int fd, unsigned request, FlatPtr arg)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    auto description = fds().file_description(fd);
    if (!description)
        return EBADF;
    if (request == FIONBIO) {
        description->set_blocking(arg == 0);
        return KSuccess;
    }
    return description->file().ioctl(*description, request, arg);
}

}

