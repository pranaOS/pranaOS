/**
 * @file unhandledinterrupthandler.cpp
 * @author Krisna Pranav
 * @brief unhandled interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "unhandledinterrupthandler.h"
#include <kernel/interrupt/unhandledinterrupthandler.h>

namespace Kernel 
{
    /**
     * @param interrupt_vector 
     */
    UnhandledInterruptHandler::UnhandledInterruptHandler(u8 interrupt_vector)
        : GenericInterruptHandler(interrupt_vector)
    { }

    /// @brief: handle_interrupt
    void UnhandledInterruptHandler::handle_interrupt(const RegisterState&)
    {
        dbg() << "Interrupt: Unhandled vector " << interrupt_number() << " was invoked for handle_interrupt(RegisterState&).";
        Processor::halt();
    }

    /**
     * @return true 
     * @return false 
     */
    [[noreturn]] bool UnhandledInterruptHandler::eoi()
    {
        dbg() << "Interrupt: Unhandled vector " << interrupt_number() << " was invoked for eoi().";
        Processor::halt();
    }
    
    /**
     * @brief Destroy the UnhandledInterruptHandler::UnhandledInterruptHandler object
     * 
     */
    UnhandledInterruptHandler::~UnhandledInterruptHandler()
    { }
}
