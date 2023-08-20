/**
 * @file spuriousinterrupthandler.cpp
 * @author Krisna Pranav
 * @brief spurious interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "spuriousinterrupthandler.h"
#include <kernel/interrupt/interruptmanagement.h>
#include <kernel/interrupt/spuriousinterrupthandler.h>

namespace Kernel
{

    /**
     * @param interrupt_number 
     */
    void SpuriousInterruptHandler::initialize(u8 interrupt_number)
    {
        new SpuriousInterruptHandler(interrupt_number);
    }

    /**
     * @return true 
     * @return false 
     */
    bool SpuriousInterruptHandler::eoi()
    {
        m_responsible_irq_controller->eoi(*this);

        return false;
    }

    /**
     * @param irq 
     */
    SpuriousInterruptHandler::SpuriousInterruptHandler(u8 irq)
        : GenericInterruptHandler(irq)
        , m_responsible_irq_controller(InterruptManagement::the().get_responsible_irq_controller(irq))
    {}

    /// @brief: enable interrupt vector
    void SpuriousInterruptHandler::enable_interrupt_vector()
    {
        if (m_enabled)
            return;
        
        m_enabled = true;

        m_responsible_irq_controller->enable(*this);
    }

    /// @breif: disable interrupt vector
    void SpuriousInterruptHandler::disable_interrupt_vector()
    {
        if (!m_enabled)
            return;
        
        m_enabled = false;

        m_responsible_irq_controller->disable(*this);
    }

    /**
     * @return const char* 
     */
    const char* SpuriousInterruptHandler::controller() const
    {
        if (m_responsible_irq_controller->type() == IRQControllerType::i82093AA)
            return "";
        
        return m_responsible_irq_controller->model();
    }

} // namepsace Kernel