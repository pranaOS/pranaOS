/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/PCSpeaker.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$beep()
{
    VERIFY_NO_PROCESS_BIG_LOCK(this);
    PCSpeaker::tone_on(440);
    auto result = Thread::current()->sleep(Time::from_nanoseconds(200'000'000));
    PCSpeaker::tone_off();
    if (result.was_interrupted())
        return EINTR;
    return 0;
}

}
