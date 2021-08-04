/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/Types.h>
#include <kernel/Arch/x86/InterruptDisabler.h>
#include <kernel/IO.h>
#include <kernel/Interrupts/GenericInterruptHandler.h>
#include <kernel/Interrupts/PIC.h>
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

UNMAP_AFTER_INIT PIC::PIC()
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

void PIC::enable(const GenericInterruptHandler& handler)
{
    InterruptDisabler disabler;
    VERIFY(!is_hard_disabled());
    VERIFY(handler.interrupt_number() >= gsi_base() && handler.interrupt_number() < interrupt_vectors_count());
    enable_vector(handler.interrupt_number());
}

void PIC::enable_vector(u8 irq)
{
    InterruptDisabler disabler;
    VERIFY(!is_hard_disabled());
    if (!(m_cached_irq_mask & (1 << irq)))
        return;
    u8 imr;
    if (irq & 8) {
        imr = IO::in8(PIC1_CMD);
        imr &= ~(1 << (irq & 7));
        IO::out8(PIC1_CMD, imr);
    } else {
        imr = IO::in8(PIC0_CMD);
        imr &= ~(1 << irq);
        IO::out8(PIC0_CMD, imr);
    }
    m_cached_irq_mask &= ~(1 << irq);
}

void PIC::eoi(const GenericInterruptHandler& handler) const
{
    InterruptDisabler disabler;
    VERIFY(!is_hard_disabled());
    u8 irq = handler.interrupt_number();
    VERIFY(irq >= gsi_base() && irq < interrupt_vectors_count());
    if ((1 << irq) & m_cached_irq_mask) {
        spurious_eoi(handler);
        return;
    }
    eoi_interrupt(irq);
}

void PIC::eoi_interrupt(u8 irq) const
{
    if (irq & 8) {
        IO::in8(PIC1_CMD); 
        IO::out8(PIC1_CTL, 0x60 | (irq & 7));
        IO::out8(PIC0_CTL, 0x60 | (2));
        return;
    }
    IO::in8(PIC0_CMD); 
    IO::out8(PIC0_CTL, 0x60 | irq);
}

void PIC::complete_eoi() const
{
    IO::out8(PIC1_CTL, 0x20);
    IO::out8(PIC0_CTL, 0x20);
}

void PIC::hard_disable()
{
    InterruptDisabler disabler;
    remap(0x20);
    IO::out8(PIC0_CMD, 0xff);
    IO::out8(PIC1_CMD, 0xff);
    m_cached_irq_mask = 0xffff;
    IRQController::hard_disable();
}

void PIC::remap(u8 offset)
{
    IO::out8(PIC0_CTL, ICW1_INIT | ICW1_ICW4);
    IO::out8(PIC1_CTL, ICW1_INIT | ICW1_ICW4);

    IO::out8(PIC0_CMD, offset);
    IO::out8(PIC1_CMD, offset + 0x08);

    IO::out8(PIC0_CMD, 1 << SLAVE_INDEX);
    IO::out8(PIC1_CMD, SLAVE_INDEX);

    IO::out8(PIC0_CMD, ICW4_8086);
    IO::out8(PIC1_CMD, ICW4_8086);

    IO::out8(PIC0_CMD, 0xff);
    IO::out8(PIC1_CMD, 0xff);
    m_cached_irq_mask = 0xffff;

    // ...except IRQ2, since that's needed for the master to let through slave interrupts.
    enable_vector(2);
}

UNMAP_AFTER_INIT void PIC::initialize()
{
    IO::out8(PIC0_CTL, ICW1_INIT | ICW1_ICW4);
    IO::out8(PIC1_CTL, ICW1_INIT | ICW1_ICW4);

    IO::out8(PIC0_CMD, IRQ_VECTOR_BASE);
    IO::out8(PIC1_CMD, IRQ_VECTOR_BASE + 0x08);

    IO::out8(PIC0_CMD, 1 << SLAVE_INDEX);
    IO::out8(PIC1_CMD, SLAVE_INDEX);

    IO::out8(PIC0_CMD, ICW4_8086);
    IO::out8(PIC1_CMD, ICW4_8086);

    IO::out8(PIC0_CMD, 0xff);
    IO::out8(PIC1_CMD, 0xff);

    enable_vector(2);

    dmesgln("PIC: Cascading mode, vectors {:#02x}-{:#02x}", IRQ_VECTOR_BASE, IRQ_VECTOR_BASE + 0xf);
}

u16 PIC::get_isr() const
{
    IO::out8(PIC0_CTL, 0x0b);
    IO::out8(PIC1_CTL, 0x0b);
    u8 isr0 = IO::in8(PIC0_CTL);
    u8 isr1 = IO::in8(PIC1_CTL);
    return (isr1 << 8) | isr0;
}

u16 PIC::get_irr() const
{
    IO::out8(PIC0_CTL, 0x0a);
    IO::out8(PIC1_CTL, 0x0a);
    u8 irr0 = IO::in8(PIC0_CTL);
    u8 irr1 = IO::in8(PIC1_CTL);
    return (irr1 << 8) | irr0;
}
}