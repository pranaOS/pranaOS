/**
 * @file dynamicparser.h
 * @author Krisna Pranav
 * @brief dynamic parser
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/refptr.h>
#include <kernel/lock.h>
#include <kernel/physical_address.h>
#include <kernel/acpi/parser.h>
#include <kernel/interrupt/irqhandler.h>
#include <kernel/vm/physicalpage.h>

namespace Kernel
{
    namespace ACPI
    {
        class DynamicParser final 
            : public IRQHandler
            , public Parser {
            
            friend class Parser;
        
        protected:  
            /**
             * @param rsdp 
             */
            explicit DynamicParser(PhysicalAddress rsdp);

        private:
            void build_namespace();

            virtual void handle_irq(const RegisterState&) override;

            OwnPtr<Region> m_acpi_namespace;
        };
    }
}