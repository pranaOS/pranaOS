/**
 * @file msihandler.h
 * @author Krisna Pranav
 * @brief msi handler
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
#include <kernel/pci/definitions.h>

namespace Kernel 
{

    class MSIHandler final : public GenericInterruptHandler 
    {
    public:
        /// @brief Destroy the MSIHandler object
        virtual ~MSIHandler();

        /// @brief: handle_interrupt
        virtual void handle_interrupt(RegisterState&) override { }

        /// @breif: enable(and)disable irq
        void enable_irq();
        void disable_irq();

        /// @brief: eoi
        /// @b: bool
        virtual bool eoi() override;

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
            return m_shared_with_others; 
        }

    protected:

        /**
         * @param irq 
         */
        void change_irq_number(u8 irq);

        //// @brief Construct a new MSIHandler object
        explicit MSIHandler(PCI::Address);

    private:
        bool m_shared_with_others;
        bool m_enabled;

    }; // class MSIHandler

} // namespace Kernel
