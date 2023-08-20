/**
 * @file pic.cpp
 * @author Krisna Pranav
 * @brief PIC
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/interrupt/pic.h>
#include <kernel/io.h>

namespace Kernel 
{

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

    /**
     * @param reg 
     * @return true 
     * @return false 
     */
    bool inline static is_all_masked(u16 reg)
    {
        return reg == 0xFFFF;
    }

    /**
     * @return true 
     * @return false 
     */
    bool PIC::is_enabled() const
    {
        return !is_all_masked(m_cached_irq_mask) && !is_hard_disabled();
    }
    
    /**
     * @param handler 
     */
    void PIC::disable(const GenericInterruptHandler& handler)
    {
        InterruptDisabler disabler;

        ASSERT(!is_hard_disabled());
        ASSERT(handler.interrupt_number() >= gsi_base() && handler.interrupt_number() < interrupt_vectors_count());

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
    
    /// @brief Construct a new PIC::PIC object
    PIC::PIC()
    {
        initialize();
    }

    /**
     * @param handler 
     */
    void PIC::spurious_eoi(const GenericInterruptHandler& handler) const
    {
        ASSERT(handler.type() == HandlerType::SpuriousInterruptHandler);

        if (handler.interrupt_number() == 7)
            return;

        if (handler.interrupt_number() == 15) {
            IO::in8(PIC1_CMD); 
            IO::out8(PIC0_CTL, 0x60 | (2));
        }
    }

    /**
     * @param irq 
     * @return true 
     * @return false 
     */
    bool PIC::is_vector_enabled(u8 irq) const
    {
        return m_cached_irq_mask & (1 << irq);
    }

    /**
     * @param handler 
     */
    void PIC::enable(const GenericInterruptHandler& handler)
    {
        InterruptDisabler disabler;

        ASSERT(!is_hard_disabled());
        ASSERT(handler.interrupt_number() >= gsi_base() && handler.interrupt_number() < interrupt_vectors_count());

        enable_vector(handler.interrupt_number());
    }

    /**
     * @param irq 
     */
    void PIC::enable_vector(u8 irq)
    {
        InterruptDisabler disabler;

        ASSERT(!is_hard_disabled());

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

    /**
     * @param handler 
     */
    void PIC::eoi(const GenericInterruptHandler& handler) const
    {
        InterruptDisabler disabler;

        ASSERT(!is_hard_disabled());

        u8 irq = handler.interrupt_number();

        ASSERT(irq >= gsi_base() && irq < interrupt_vectors_count());

        if ((1 << irq) & m_cached_irq_mask) {
            spurious_eoi(handler);
            return;
        }

        eoi_interrupt(irq);
    }

    /**
     * @param irq 
     */
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

    /// @breif: complete_eoi
    void PIC::complete_eoi() const
    {
        IO::out8(PIC1_CTL, 0x20);
        IO::out8(PIC0_CTL, 0x20);
    }   

    /// @breif: hard_disable
    void PIC::hard_disable()
    {
        InterruptDisabler disabler;

        remap(0x20);

        IO::out8(PIC0_CMD, 0xff);
        IO::out8(PIC1_CMD, 0xff);
        
        m_cached_irq_mask = 0xffff;

        IRQController::hard_disable();
    }

    /**
     * @param offset 
     */
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

        enable_vector(2);
    }

    /// @breif: initialize
    void PIC::initialize()
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

        klog() << "PIC: cascading mode, vectors 0x" << String::format("%x", IRQ_VECTOR_BASE) << "-0x" << String::format("%x", IRQ_VECTOR_BASE + 0xf);
    }

    /**
     * @return u16 
     */
    u16 PIC::get_isr() const
    {
        IO::out8(PIC0_CTL, 0x0b);
        IO::out8(PIC1_CTL, 0x0b);

        u8 isr0 = IO::in8(PIC0_CTL);
        u8 isr1 = IO::in8(PIC1_CTL);

        return (isr1 << 8) | isr0;
    }

    /**
     * @return u16 
     */
    u16 PIC::get_irr() const
    {
        IO::out8(PIC0_CTL, 0x0a);
        IO::out8(PIC1_CTL, 0x0a);

        u8 irr0 = IO::in8(PIC0_CTL);
        u8 irr1 = IO::in8(PIC1_CTL);

        return (irr1 << 8) | irr0;
    }
} // namespace Kernel
