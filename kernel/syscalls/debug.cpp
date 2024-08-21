/**
 * @file debug.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief debug
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/io.h>
#include <kernel/ksyms.h>
#include <kernel/process.h>
#include <kernel/userorkernelbuffer.h>

namespace Kernel 
{

    /**
     * @brief dump bactrace sys
     * 
     */
    int Process::sys$dump_backtrace()
    {
        dump_backtrace();
        return 0;
    }

    /**
     * @brief dbgputch sys
     * 
     */
    int Process::sys$dbgputch(u8 ch)
    {
        IO::out8(0xe9, ch);
        return 0;
    }

    /**
     * @brief dbgputstr sys 
     * 
     */
    int Process::sys$dbgputstr(Userspace<const u8*> characters, int length)
    {
        if (length <= 0)
            return 0;

        SmapDisabler disabler;

        auto buffer = UserOrKernelBuffer::for_user_buffer(characters, length);

        if (!buffer.has_value())
            return -EFAULT;
            
        ssize_t nread = buffer.value().read_buffered<1024>(length, [&](const u8* buffer, size_t buffer_size) {
            for (size_t i = 0; i < buffer_size; ++i)
                IO::out8(0xe9, buffer[i]);
            return (ssize_t)buffer_size;
        });

        if (nread < 0)
            return (int)nread;

        return 0;
    }

} // namespace Kernel
