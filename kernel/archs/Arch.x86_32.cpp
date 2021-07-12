/*
 * Copyright (c) 2021, krishpranav, johnsmith5c12
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/x86/COM.h"
#include "archs/x86/CPUID.h"
#include "archs/x86/FPU.h"
#include "archs/x86/Power.h"
#include "archs/x86/RTC.h"
#include "archs/x86/x86.h"
#include "archs/x86_32/GDT.h"
#include "archs/x86_32/Interrupts.h"
#include "archs/x86_32/Paging.h"
#include "archs/x86_32/x86_32.h"
#include "system/Streams.h"
#include "system/graphics/EarlyConsole.h"

namespace Arch
{

void disable_interrupts() { x86::cli(); }

void enable_interrupts() { x86::sti(); }

void halt() { x86::hlt(); }

void yield() { asm("int $127"); }

void save_context(Task *task)
{
    fpu_save_context(task);
}

void load_content(Task *task)
{
    fpu_load_context(task);
    x86_32::set_kernel_stack((uintptr_t)task->kernel_stack + PROCESS_STACK_SIZE);
}


void task_go(Task *task)
{
    fpu_init_context(task);

    if (task->_flags & TASK_USER)
    {
        x86_32::UserInterruptStackFrame stackframe = {};

        stackframe.user_esp = task->user_stack_pointer;

        stackframe.eflags = 0x202;
        stackframe.eip = (uintptr_t)task->entry_point;
        stackframe.ebp = 0;

        stackframe.cs = 0x1b;
        stackframe.ds = 0x23;
        stackframe.es = 0x23;
        stackframe.fs = 0x23;
        stackframe.gs = 0x23;
        stackframe.ss = 0x23;

        task_kernel_stack_push(task, &stackframe, sizeof(x86_32::UserInterruptStackFrame));
    }
    else
    {
        x86_32::InterruptStackFrame stackframe = {};

        stackframe.eflags = 0x202;
        stackframe.eip = (uintptr_t)task->entry_point;
        stackframe.ebp = 0;

        stackframe.cs = 0x08;
        stackframe.ds = 0x10;
        stackframe.es = 0x10;
        stackframe.fs = 0x10;
        stackframe.gs = 0x10;

        task_kernel_stack_push(task, &stackframe, sizeof(x86_32::InterruptStackFrame));
    }
}

size_t debug_write(const void *buffer, size_t size) { return com_write(COM1, buffer, size); }

TimeStamp get_time() { return rtc_now(); }

NO_RETURN void reboot()
{
    early_console_enable();
    Kernel::logln("Rebooting...");

    x86::reboot_8042();
    x86::reboot_triple_fault();

    Kernel::logln("Failed to reboot: Halting!");
    system_stop();
}

NO_RETURN void shutdown()
{
    early_console_enable();
    Kernel::logln("Shutting down...");

    x86::shutdown_virtual_machines();
    x86::shutdown_acpi();

    Kernel::logln("Failed to shutdown: Halting!");
    system_stop();
}

struct Stackframe
{
    Stackframe *ebp;
    uint32_t eip;
};


}