/**
 * @file unhandledinterrupthandler.h
 * @author Krisna Pranav
 * @brief unhandled interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

        /// @brief Destroy the UnhandledInterruptHandler object
        virtual ~UnhandledInterruptHandler();

        /// @breif: handle_interrupt
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
            return "Unhandled Interrupt Handler"; 
        }

        /**
         * @return const char* 
         */
        virtual const char* controller() const override 
        { 
            ASSERT_NOT_REACHED(); 
        }

        /**
         * @return size_t 
         */
        virtual size_t sharing_devices_count() const override 
        { 
            return 0; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_shared_handler() const override 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_sharing_with_others() const override 
        { 
            return false; 
        }

    private:
    }; // class UnhandledInterruptHandler

} // namespace Kernel
