/**
 * @file beep.cpp
 * @author Krisna Pranav
 * @brief beep
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/device/pcspeaker.h>

namespace Kernel 
{
    /**
     * @brief Process beep sys
     * 
     */
    int Process::sys$beep()
    {
        PCSpeaker::tone_on(440);

        auto result = Thread::current()->sleep({ 0, 200 });

        PCSpeaker::tone_off();

        if (result.was_interrupted())
            return -EINTR;

        return 0;
    }

} // namespace Kernel