/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <kernel/arch/x86/CPU.h>
#include <kernel/Panic.h>
#include <kernel/Process.h>

void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    asm volatile("cli");
    critical_dmesgln("ASSERTION FAILED: {}", msg);
    critical_dmesgln("{}:{} in {}", file, line, func);

    abort();
}

[[noreturn]] void abort()
{
    auto process = Process::current();
    if (process)
        MM.enter_process_paging_scope(*process);

    PANIC("Aborted");
}

[[noreturn]] void _abort()
{
    asm volatile("ud2");
    __builtin_unreachable();
}
