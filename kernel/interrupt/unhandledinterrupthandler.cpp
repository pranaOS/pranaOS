/**
 * @file unhandledinterrupthandler.cpp
 * @author Krisna Pranav
 * @brief unhandled interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
    {}

    /// @brief: handle_interrupt
    void UnhandledInterruptHandler::handle_interrupt(const RegisterState)
    {
        dbg() << "Interrupt: unhanded vector " << interrupt_number() << "was invoked for handle_interrupt.";
        Processor::halt(); 
    }

    UnhandledInterruptHandler::~UnhandledInterruptHandler()
    {}

} // namespace Kernel