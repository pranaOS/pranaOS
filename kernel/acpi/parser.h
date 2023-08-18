/**
 * @file parser.h
 * @author Krisna Pranav
 * @brief parser
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <kernel/virtual_address.h>
#include <kernel/acpi/definitions.h>
#include <kernel/acpi/initialize.h>
#include <kernel/filesystem/file.h>
#include <kernel/vm/region.h>
#include <mods/types.h>

namespace Kernel 
{
    namespace ACPI 
    {
        class Parser 
        {
        public:
            static Parser* the();

            /**
             * @tparam ParserType 
             * @param rsdp 
             */
            template<typename ParserType>
            static void initialize(PhysicalAddress rsdp)
            {
                set_the(*new ParserType(rsdp));
            }

            /**
             * @param signature 
             * @return PhysicalAddress 
             */
            virtual PhysicalAddress find_table(const StringView& signature);

            virtual void try_acpi_reboot();
            virtual bool can_reboot();
            virtual void try_acpi_shutdown();

            /**
             * @return true 
             * @return false 
             */
            virtual bool can_shutdown() 
            { 
                return false; 
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool have_8042() const
            {
                return m_x86_specific_flags.keyboard_8042;
            }
            
            /**
             * @return const FADTFlags::HardwareFeatures& 
             */
            const FADTFlags::HardwareFeatures& hardware_features() const 
            { 
                return m_hardware_flags; 
            }
            
            /**
             * @return const FADTFlags::x86_Specific_Flags& 
             */
            const FADTFlags::x86_Specific_Flags& x86_specific_flags() const 
            { 
                return m_x86_specific_flags; 
            }

            virtual void enable_aml_interpretation();
            virtual void enable_aml_interpretation(File&);
            virtual void enable_aml_interpretation(u8*, u32);
            virtual void disable_aml_interpretation();

        protected:
            explicit Parser(PhysicalAddress rsdp);

        private:
            static void set_the(Parser&);

            void locate_static_data();
            void locate_main_system_description_table();
            void initialize_main_system_description_table();

            size_t get_table_size(PhysicalAddress);

            u8 get_table_revision(PhysicalAddress);

            void init_fadt();
            void init_facs();

            bool validate_reset_register();

            /**
             * @param value 
             */
            void access_generic_address(const Structures::GenericAddressStructure&, u32 value);

            PhysicalAddress m_rsdp;
            PhysicalAddress m_main_system_description_table;

            Vector<PhysicalAddress> m_sdt_pointers;
            PhysicalAddress m_fadt;
            PhysicalAddress m_facs;

            bool m_xsdt_supported { false };
            
            FADTFlags::HardwareFeatures m_hardware_flags;
            FADTFlags::x86_Specific_Flags m_x86_specific_flags;
        }; // class Parser
    } // namespace ACPI 
} // namespace Kernel
