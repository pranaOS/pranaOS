/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <kernel/acpi/DynamicParser.h>
#include <kernel/acpi/Parser.h>
#include <kernel/Sections.h>

namespace Kernel::ACPI {

UNMAP_AFTER_INIT DynamicParser::DynamicParser(PhysicalAddress rsdp)
    : IRQHandler(9)
    , Parser(rsdp)
{
    dmesgln("ACPI: Dynamic Parsing Enabled, Can parse AML");
}

bool DynamicParser::handle_irq(const RegisterState&)
{
    VERIFY_NOT_REACHED();
}

void DynamicParser::enable_aml_interpretation()
{
    VERIFY_NOT_REACHED();
}
void DynamicParser::enable_aml_interpretation(File&)
{
    VERIFY_NOT_REACHED();
}
void DynamicParser::enable_aml_interpretation(u8*, u32)
{
    VERIFY_NOT_REACHED();
}
void DynamicParser::disable_aml_interpretation()
{
    VERIFY_NOT_REACHED();
}
void DynamicParser::try_acpi_shutdown()
{
    VERIFY_NOT_REACHED();
}

void DynamicParser::build_namespace()
{
    VERIFY_NOT_REACHED();
}

}
