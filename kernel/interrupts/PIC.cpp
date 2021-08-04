/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/Types.h>
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/IO.h>
#include <kernel/interrupts/GenericInterruptHandler.h>
#include <kernel/interrupts/PIC.h>
#include <kernel/Sections.h>

namespace Kernel {

#define SLAVE_INDEX 2

#define PIC0_CTL 0x20
#define PIC0_CMD 0x21
#define PIC1_CTL 0xA0
#define PIC1_CMD 0xA1

#define ICW1_ICW4 0x01      
#define ICW1_SINGLE 0x02    
#define ICW1_INTERVAL4 0x04 
#define ICW1_LEVEL 0x08     
#define ICW1_INIT 0x10     

#define ICW4_8086 0x01      
#define ICW4_AUTO 0x02      
#define ICW4_BUF_SLAVE 0x08 
#define ICW4_BUF_MASTER 0x0C 
#define ICW4_SFNM 0x10       

bool inline static is_all_masked(u16 reg)
{
    return reg == 0xFFFF;
}

bool PIC::is_enabled() const 
{
    return !is_all_masked(m_cached_irq_mask) && !is_hard_disabled();
}

void PIC::disable(const GenericInterruptHandler& handler)
{
    InterruptDisabler disabler;
    VERIFY(!is_hard_disabled());
    VERIFY(handler.interrupt_number() >= gsi_base() && handler.interrupt_number() < interrupt_vectors_count());
    u8 irq = handler.interrupt_number();
    if (m_cached_irq_mask & (1 << irq))
        return;
    u8 imr;
    if (irq & 8) {
        imr = IO::in8(PIC1_CMD);
        imr |= 1 << (irq & 7);
        IO::out8(PIC1_CMD, imr);
    } else {
        imr = IO::in8(PIC0_CMD);
        imr |= 1 << irq;
        IO::out8(PIC0_CMD, imr);
    }
    m_cached_irq_mask |= 1 << irq;
}

UNMAP_AFTER_INNIT PIC::PIC()
{
    initialize();
}

void PIC::spurious_eoi(const GenericInterruptHandler& handler) const
{
    VERIFY(handler.type() == HandlerType::SpuriousInterruptHandler);
    if (handler.interrupt_number() == 7)
        return;
    if (handler.interrupt_number() == 15) {
        IO::in8(PIC1_CMD); 
        IO::out8(PIC0_CTL, 0x60 | (2));
    }
}

bool PIC::is_vector_enabled(u8 irq) const
{
    return m_cached_irq_mask & (1 << irq);
}

}