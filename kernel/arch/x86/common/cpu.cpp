/**
 * @file cpu.cpp
 * @author Krisna Pranav
 * @brief CPU
 * @version 6.0
 * @date 2024-09-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <kernel/arch/x86/cpu.h>
#include <kernel/panic.h>
#include <kernel/process.h>

using namespace Kernel;

/**
 * @param msg 
 * @param file 
 * @param line 
 * @param func 
 */
void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    asm volatile("cli");
    critical_dmesgln("ASSERTION FAILED: {}", msg);
    critical_dmesgln("{}:{} in {}", file, line, func);

    abort();
}

[[noreturn]] void abort()
{
    auto thread = Thread::current();
    if (thread)
        thread->set_crashing();
        
    if (Process::has_current())
        Memory::MemoryManager::enter_process_address_space(Process::current());

    PANIC("Aborted");
}

[[noreturn]] void _abort()
{
    asm volatile("ud2");
    __builtin_unreachable();
}
