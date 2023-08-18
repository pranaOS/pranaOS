/**
 * @file dynamicparser.cpp
 * @author Krisna Pranav
 * @brief dynamic parser
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "dynamicparser.h"
#include <kernel/acpi/parser.h>
#include <kernel/acpi/dynamicparser.h>

namespace Kernel 
{
    namespace ACPI 
    {

        /**
         * @param rsdp 
         */
        DynamicParser::DynamicParser(PhysicalAddress rsdp)
            : IRQHandler(9)
            , Parser(rsdp)
        {
            klog() << "[ACPI]: Dynamic Parsing Enabled, Now it can parse AML";
        }

        void DynamicParser::handle_irq(const RegisterState&)
        {
            ASSERT_NOT_REACHED();
        }

        void DynamicParser::enable_aml_interpretation()
        {
            ASSERT_NOT_REACHED();
        }
        void DynamicParser::enable_aml_interpretation(File&)
        {
            ASSERT_NOT_REACHED();
        }
        void DynamicParser::enable_aml_interpretation(u8*, u32)
        {
            ASSERT_NOT_REACHED();
        }
        void DynamicParser::disable_aml_interpretation()
        {
            ASSERT_NOT_REACHED();
        }

        void DynamicParser::try_acpi_shutdown()
        {
            ASSERT_NOT_REACHED();
        }

        void DynamicParser::build_namespace()
        {
            ASSERT_NOT_REACHED();
        }

    } // namespace ACPI 

} // namespace Kernel
