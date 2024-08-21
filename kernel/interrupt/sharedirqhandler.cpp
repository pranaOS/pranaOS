/**
 * @file sharedirqhandler.cpp
 * @author Krisna Pranav
 * @brief shared irq handler
 * @version 6.0 
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "sharedirqhandler.h"
#include <kernel/arch/i386/cpu.h>
#include <kernel/assertions.h>
#include <kernel/interrupt/irqhandler.h>
#include <kernel/interrupt/interruptmanagement.h>
#include <kernel/interrupt/pic.h>
#include <kernel/interrupt/sharedirqhandler.h>

namespace Kernel 
{

    /**
     * @param interrupt_number 
     */
    void SharedIRQHandler::initialize(u8 interrupt_number)
    {
        new SharedIRQHandler(interrupt_number);
    }

    /**
     * @param handler 
     */
    void SharedIRQHandler::register_handler(GenericInterruptHandler& handler)
    {
    #ifdef INTERRUPT_DEBUG
        klog() << "Interrupt Handler registered @ Shared Interrupt Handler " << interrupt_number();
    #endif

        m_handlers.set(&handler);
        enable_interrupt_vector();
    }

    /**
     * @param handler 
     */
    void SharedIRQHandler::unregister_handler(GenericInterruptHandler& handler)
    {
    #ifdef INTERRUPT_DEBUG
        klog() << "Interrupt Handler unregistered @ Shared Interrupt Handler " << interrupt_number();
    #endif

        m_handlers.remove(&handler);
        if (m_handlers.is_empty())
            disable_interrupt_vector();
    }

    /**
     * @return true 
     * @return false 
     */
    bool SharedIRQHandler::eoi()
    {
    #ifdef INTERRUPT_DEBUG
        dbg() << "EOI IRQ " << interrupt_number();
    #endif

        m_responsible_irq_controller->eoi(*this);
        return true;
    }

    /**
     * @param irq 
     */
    SharedIRQHandler::SharedIRQHandler(u8 irq)
        : GenericInterruptHandler(irq)
        , m_responsible_irq_controller(InterruptManagement::the().get_responsible_irq_controller(irq))
    {

    #ifdef INTERRUPT_DEBUG
        klog() << "Shared Interrupt Handler registered @ " << interrupt_number();
    #endif
    
        disable_interrupt_vector();
    }

    /// @brief Destroy the SharedIRQHandler::SharedIRQ Handler object
    SharedIRQHandler::~SharedIRQHandler()
    {
    #ifdef INTERRUPT_DEBUG
        klog() << "Shared Interrupt Handler unregistered @ " << interrupt_number();
    #endif

        disable_interrupt_vector();
    }

    /**
     * @param regs 
     */
    void SharedIRQHandler::handle_interrupt(const RegisterState& regs)
    {
        ASSERT_INTERRUPTS_DISABLED();

    #ifdef INTERRUPT_DEBUG
        dbg() << "Interrupt @ " << interrupt_number();
        dbg() << "Interrupt Handlers registered - " << m_handlers.size();
    #endif

        int i = 0;

        for (auto* handler : m_handlers) {
    #ifdef INTERRUPT_DEBUG
            dbg() << "Going for Interrupt Handling @ " << i << ", Shared Interrupt " << interrupt_number();
    #endif
    
            ASSERT(handler != nullptr);
            handler->increment_invoking_counter();
            handler->handle_interrupt(regs);

    #ifdef INTERRUPT_DEBUG
            dbg() << "Going for Interrupt Handling @ " << i << ", Shared Interrupt " << interrupt_number() << " - End";
    #endif

            i++;
        }
    }

    /// @brief: enable interrupt vecotr
    void SharedIRQHandler::enable_interrupt_vector()
    {
        if (m_enabled)
            return;

        m_enabled = true;
        m_responsible_irq_controller->enable(*this);
    }

    /// @breif: disable interrupt vector
    void SharedIRQHandler::disable_interrupt_vector()
    {
        if (!m_enabled)
            return;
            
        m_enabled = false;
        m_responsible_irq_controller->disable(*this);
    }

} // namespace Kernel
