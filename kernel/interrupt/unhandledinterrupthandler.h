/**
 * @file unhandledinterrupthandler.h
 * @author Krisna Pranav
 * @brief unhandled interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/genericinterrupthandler.h>

namespace Kernel
{

    class UnhandledInterruptHandler final : public GenericInterruptHandler
    {
    public:
        /**
         * @param interrupt_vector 
         */
        explicit UnhandledInterruptHandler(u8 interrupt_vector);
        
        /// @brief: handle_interrupt
        virtual void handle_interrupt(const RegisterState&) override;

        /**
         * @return true 
         * @return false 
         */
        [[noreturn]] virtual bool eoi() override;

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const override 
        { 
            return HandlerType::UnhandledInterruptHandler; 
        }

        /**
         * @return const char* 
         */
        virtual const char* purpose() const override
        {
            return "Unhandled inheritance";
        }

        /**
         * @return const char* 
         */
        virtual const char* controller() const override
        {
            ASSERT_NOT_REACHED();
        }

    private:
    }; // class UnhandledInterruptHandler

} // namespace Kernel 