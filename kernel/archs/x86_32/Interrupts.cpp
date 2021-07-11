/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <assert.h>
#include "system/interrupts/Dispatcher.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Syscalls.h"
#include "archs/x86/PIC.h"
#include "archs/x86_32/Interrupts.h"
#include "archs/x86_32/x86_32.h"


namespace Arch::x86_32
{

static const char *_exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

}