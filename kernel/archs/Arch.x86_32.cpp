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

}