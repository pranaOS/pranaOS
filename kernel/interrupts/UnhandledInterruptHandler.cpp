/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/interrupts/UnhandledInterruptHandler.h>
#include <kernel/Panic.h>

namespace Kernel {
UnhandledInterruptHandler::UnhandledInterruptHandler(u8 interrupt_vector)
    : GenericInterruptHandler(interrupt_vector)
{
}

bool UnhandledInterruptHandler::handle_interrupt(const RegisterState&)
{
    PANIC("Interrupt: Unhandled vector {} was invoked for handle_interrupt(RegisterState&).", interrupt_number());
}

[[noreturn]] bool UnhandledInterruptHandler::eoi()
{
    PANIC("Interrupt: Unhandled vector {} was invoked for eoi().", interrupt_number());
}

UnhandledInterruptHandler::~UnhandledInterruptHandler()
{
}
}