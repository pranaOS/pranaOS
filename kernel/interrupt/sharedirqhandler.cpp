/**
 * @file sharedirqhandler.cpp
 * @author Krisna Pranav
 * @brief shared irq handler
 * @version 6.0 
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        #ifndef INTERRUPT_DEBUG
            klog() << "Interrupt handler register, shared interrupt handler";
        #endif
        
            m_handlers.set(&handler);
            enable_interrupt_vector();
    }

} // namespace Kernel 