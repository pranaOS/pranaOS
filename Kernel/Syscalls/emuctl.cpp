/*
 * Copyright (c) 2021, krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/Process.h>

namespace Kernel {

KResultOr<int> Process::sys$emuctl()
{
    return ENOSYS;
}

}
