/**
 * @file sharedirqhandler.h
 * @author Krisna Pranav
 * @brief shared irq handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/genericinterrupthandler.h>

namespace Kernel 
{
    class IRQHandler;

    class SharedIRQHandler final : public GenericInterruptHandler 
    {
    public:

        /**
         * @param interrupt_number 
         */
        static void initialize(u8 interrupt_number);

        /// @brief Destroy the SharedIRQHandler object
        virtual ~SharedIRQHandler();

        /**
         * @param regs 
         */
        virtual void handle_interrupt(const RegisterState& regs) override;

        /// @brief register(&)unregister handler
        void register_handler(GenericInterruptHandler&);
        void unregister_handler(GenericInterruptHandler&);

        /**
         * @return true 
         * @return false 
         */
        virtual bool eoi() override;

        /**
         * @return size_t 
         */
        virtual size_t sharing_devices_count() const override 
        { 
            return m_handlers.size(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_shared_handler() const override 
        { 
            return true; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_sharing_with_others() const override 
        { 
            return false; 
        }

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const override 
        { 
            return HandlerType::SharedIRQHandler; 
        }

        /**
         * @return const char* 
         */
        virtual const char* purpose() const override 
        { 
            return "Shared IRQ Handler"; 
        }

        /**
         * @return const char* 
         */
        virtual const char* controller() const override 
        { 
            return m_responsible_irq_controller->model(); 
        }

    private:

        void enable_interrupt_vector();
        void disable_interrupt_vector();

        /**
         * @param interrupt_number 
         */
        explicit SharedIRQHandler(u8 interrupt_number);

        bool m_enabled;

        HashTable<GenericInterruptHandler*> m_handlers;

        RefPtr<IRQController> m_responsible_irq_controller;
        
    }; // class SharedIRQHandler

} // namespace Kernel
