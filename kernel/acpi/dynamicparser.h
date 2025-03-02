/**
 * @file dynamicparser.h
 * @author Krisna Pranav
 * @brief dynamic parser
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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

        public:

            /// @breif: enable aml interpretation
            virtual void enable_aml_interpretation() override;

            /**
             * @param dsdt_file 
             */
            virtual void enable_aml_interpretation(File& dsdt_file) override;

            /**
             * @param physical_dsdt 
             * @param dsdt_payload_legnth 
             */
            virtual void enable_aml_interpretation(u8* physical_dsdt, u32 dsdt_payload_legnth) override;

            /// @breif: disable aml interpretation
            virtual void disable_aml_interpretation() override;

            /// @breif: try acpi shutdown
            virtual void try_acpi_shutdown() override;

            /**
             * @return true 
             * @return false 
             */
            virtual bool can_shutdown() override 
            { 
                return true; 
            }

            /**
             * @return const char* 
             */
            virtual const char* purpose() const override 
            { 
                return "ACPI Parser"; 
            }

        protected:
            /**
             * @param rsdp 
             */
            explicit DynamicParser(PhysicalAddress rsdp);

        private:
            /// @brief: build namespace
            void build_namespace();
            
            /// @breif: handle_irq
            virtual void handle_irq(const RegisterState&) override;

            OwnPtr<Region> m_acpi_namespace;
        }; // class DynamicParser

    } // namespace ACPI 

} // namespace Kernel
