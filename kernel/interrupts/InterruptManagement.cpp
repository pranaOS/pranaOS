/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/acpi/MultiProcessorParser.h>
#include <kernel/api/Syscall.h>
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/arch/x86/Interrupts.h>
#include <kernel/CommandLine.h>
#include <kernel/IO.h>
#include <kernel/interrupts/APIC.h>
#include <kernel/interrupts/IOAPIC.h>
#include <kernel/interrupts/InterruptManagement.h>
#include <kernel/interrupts/PIC.h>
#include <kernel/interrupts/SharedIRQHandler.h>
#include <kernel/interrupts/SpuriousInterruptHandler.h>
#include <kernel/Sections.h>
#include <kernel/vm/TypedMapping.h>

#define PCAT_COMPAT_FLAG 0x1

namespace Kernel {

bool InterruptManagement::initialized()
{
    return (s_interrupt_management != nullptr);
}

InterruptManagement& InterruptManagement::the()
{
    VERIFY(InterruptManagement::initialized());
    return *s_interrupt_management;
}

UNMAP_AFTER_INIT void InterruptManagement::initialize()
{
    VERIFY(!InterruptManagement::initialized());
    s_interrupt_management = new InterruptManagement();

    if (kernel_command_line().is_smp_enabled())
        InterruptManagement::the().switch_to_ioapic_mode();
    else
        InterruptManagement::the().switch_to_pic_mode();
}

void InterruptManagement::enumerate_interrupt_handlers(Function<void(GenericInterruptHandler&)> callback)
{
    for (int i = 0; i < GENERIC_INTERRUPT_HANDLERS_COUNT; i++) {
        auto& handler = get_interrupt_handler(i);
        if (handler.type() == HandlerType::SharedIRQHandler) {
            static_cast<SharedIRQHandler&>(handler).enumerate_handlers(callback);
            continue;
        }
        if (handler.type() != HandlerType::UnhandledInterruptHandler)
            callback(handler);
    }
}

IRQController& InterruptManagement::get_interrupt_controller(int index)
{
    VERIFY(index >= 0);
    VERIFY(!m_interrupt_controllers[index].is_null());
    return *m_interrupt_controllers[index];
}

u8 InterruptManagement::acquire_mapped_interrupt_number(u8 original_irq)
{
    if (!InterruptManagement::initialized()) {

        return original_irq;
    }
    return InterruptManagement::the().get_mapped_interrupt_vector(original_irq);
}

u8 InterruptManagement::acquire_irq_number(u8 mapped_interrupt_vector)
{
    VERIFY(InterruptManagement::initialized());
    return InterruptManagement::the().get_irq_vector(mapped_interrupt_vector);
}

u8 InterruptManagement::get_mapped_interrupt_vector(u8 original_irq)
{
    VERIFY((original_irq + IRQ_VECTOR_BASE) != syscall_vector);
    return original_irq;
}

u8 InterruptManagement::get_irq_vector(u8 mapped_interrupt_vector)
{
    return mapped_interrupt_vector;
}

RefPtr<IRQController> InterruptManagement::get_responsible_irq_controller(u8 interrupt_vector)
{
    if (m_interrupt_controllers.size() == 1 && m_interrupt_controllers[0]->type() == IRQControllerType::i8259) {
        return m_interrupt_controllers[0];
    }
    for (auto& irq_controller : m_interrupt_controllers) {
        if (irq_controller->gsi_base() <= interrupt_vector)
            if (!irq_controller->is_hard_disabled())
                return irq_controller;
    }
    VERIFY_NOT_REACHED();
}

}