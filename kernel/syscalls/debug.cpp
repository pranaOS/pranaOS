/**
 * @file debug.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief debug
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/io.h>
#include <kernel/ksyms.h>
#include <kernel/process.h>
#include <kernel/userorkernelbuffer.h>

namespace Kernel
{
    int Process::sys$dump_backtrace()
    {
        dump_backtrace();
        return 0;
    }
    
    int Process::sys$dbgputch(u8 ch)
    {
        IO::out8(0xe9, ch);

        return 0;
    }
} // namespace Kernel 