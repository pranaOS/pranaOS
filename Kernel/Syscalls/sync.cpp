/*
 * Copyright (c) 2018-2020, krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/FileSystem/VirtualFileSystem.h>
#include <Kernel/Process.h>

namespace Kernel {

KResultOr<int> Process::sys$sync()
{
    REQUIRE_PROMISE(stdio);
    VFS::the().sync();
    return 0;
}

}
