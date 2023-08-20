/**
 * @file spuriousinterrupthandler.h
 * @author Krisna Pranav
 * @brief spurious interrupt handler
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/ownptr.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/interrupt/irqcontroller.h>

namespace Kernel 
{

    class SpuriousInterruptHandler final : public GenericInterruptHandler 
    {
    public:
        /**
         * @param interrupt_number 
         */
        static void initialize(u8 interrupt_number);

        /// @brief Destroy the SpuriousInterruptHandler object
        virtual ~SpuriousInterruptHandler();

        /**
         * @param regs 
         */
        virtual void handle_interrupt(const RegisterState& regs) override;

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
            return 1; 
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

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const override 
        { 
            return HandlerType::SpuriousInterruptHandler; 
        }

        /**
         * @return const char* 
         */
        virtual const char* purpose() const override 
        { 
            return "Spurious Interrupt Handler"; 
        }

        /**
         * @return const char* 
         */
        virtual const char* controller() const override;

    private:
        void enable_interrupt_vector();
        void disable_interrupt_vector();

        /**
         * @param interrupt_number 
         */
        explicit SpuriousInterruptHandler(u8 interrupt_number);

        bool m_enabled;

        RefPtr<IRQController> m_responsible_irq_controller;

        OwnPtr<GenericInterruptHandler> m_real_handler;

    }; // class SpuriousInterruptHandler

} // namespace Kernel
