/*
 * Copyright (c) 2021, krishpranav, johnsmith5c12
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/Arch.h"
#include "archs/x86/COM.h"
#include "archs/x86/CPUID.h"
#include "archs/x86/FPU.h"
#include "archs/x86/Power.h"
#include "archs/x86/RTC.h"
#include "archs/x86_64/GDT.h"
#include "archs/x86_64/Interrupts.h"
#include "archs/x86_64/Paging.h"
#include "archs/x86_64/x86_64.h"

namespace Arch
{

void disable_interrupts() { x86::cli(); }

void enable_interrupts() { x86::sti(); }

void halt()
{
    x86::hlt();
}

void yield()
{
    asm("int $127");
}

void save_context(Task *task)
{
    fpu_save_context(task);
}

void load_context(Task *task)
{
    fpu_load_context(task);
    x86_64::set_kernel_stack((uint64_t)task->kernel_stack + PROCESS_STACK_SIZE);

}

void task_go(Task *task)
{
    if (task->_flags & TASK_USER)
    {
        x86_64::InterruptStackFrame stackframe = {};

        stackframe.rsp = (uintptr_t)task->user_stack_pointer;

        stackframe.rflags = 0x202;
        stackframe.rip = (uintptr_t)task->entry_point;
        stackframe.rbp = (uintptr_t)stackframe.rsp;

        stackframe.cs = 0x1b;
        stackframe.ss = 0x23;

        task_kernel_stack_push(task, &stackframe, sizeof(x86_64::InterruptStackFrame));
    }
    else
    {
        x86_64::InterruptStackFrame stackframe = {};

        stackframe.rsp = (uintptr_t)task->kernel_stack_pointer - sizeof(x86_64::InterruptStackFrame);

        stackframe.rflags = 0x202;
        stackframe.rip = (uintptr_t)task->entry_point;
        stackframe.rbp = (uintptr_t)stackframe.rsp;

        stackframe.cs = 0x08;
        stackframe.ss = 0x10;

        task_kernel_stack_push(task, &stackframe, sizeof(x86_64::InterruptStackFrame));
    }
}

size_t debug_write(const void *buffer, size_t size)
{
    return com_write(COM1, buffer, size);
}

TimeStamp get_time()
{
    return rtc_now();   
}

NO_RETURN void reboot()
{
    early_console_enable();
    Kernel::logln("Rebooting...");

    x86::reboot_8042();
    x86::reboot_triple_fault();

    Kernel::logln("Failed to reboot: Halting!");
    system_stop();
}

}