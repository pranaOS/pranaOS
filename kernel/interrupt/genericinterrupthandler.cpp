/**
 * @file genericinterrupthandler.cpp
 * @author Krisna Pranav
 * @brief generic interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/arch/i386/cpu.h>
#include <kernel/assertions.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/interrupt/interruptmanagement.h>
#include <kernel/interrupt/pic.h>

namespace Kernel
{
    /**
     * @param interrupt_number 
     * @return GenericInterruptHandler& 
     */
    GenericInterruptHandler& GenericInterruptHandler::from(u8 interrupt_number)
    {
        return get_interrupt_handler(interrupt_number);
    }

    /// @brief Destroy the GenericInterruptHandler::GenericInterruptHandler object
    GenericInterruptHandler::~GenericInterruptHandler()
    {}

    /**
     * @param number 
     */
    void GenericInterruptHandler::change_interrupt_number(u8 number)
    {
        ASSERT_INTERRUPTS_DISABLED();

        ASSERT(!m_disable_remap);

        unregister_generic_interrupt_handler(InterruptManagement::acquire_mapped_interrupt_number(interrupt_number()), *this);
        m_interrupt_number = number;
        register_generic_interrupt_handler(InterruptManagement::acquire_mapped_interrupt_number(interrupt_number()), *this);
    }
    
} // namespace Kernel