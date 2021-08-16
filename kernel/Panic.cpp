/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Format.h>
#include <kernel/arch/x86/Processor.h>
#include <kernel/CommandLine.h>
#include <kernel/IO.h>
#include <kernel/KSyms.h>
#include <kernel/Panic.h>

namespace Kernel {

[[noreturn]] static void __shutdown()
{

    IO::out16(0x604, 0x2000);
    IO::out16(0xb004, 0x2000);
    Processor::halt();
}

void __panic(const char* file, unsigned int line, const char* function)
{
    critical_dmesgln("at {}:{} in {}", file, line, function);
    dump_backtrace(PrintToScreen::Yes);
    if (kernel_command_line().boot_mode() == BootMode::SelfTest)
        __shutdown();
    else
        Processor::halt();
}
}
