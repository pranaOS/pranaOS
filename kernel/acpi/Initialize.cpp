/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/acpi/DynamicParser.h>
#include <kernel/CommandLine.h>
#include <kernel/Sections.h>

namespace Kernel::ACPI {

UNMAP_AFTER_INIT void initialize()
{
    auto feature_level = kernel_command_line().acpi_feature_level();
    if (feature_level == AcpiFeatureLevel::Disabled)
        return;

    auto rsdp = StaticParsing::find_rsdp();
    if (!rsdp.has_value())
        return;

    if (feature_level == AcpiFeatureLevel::Enabled)
        Parser::initialize<DynamicParser>(rsdp.value());
    else
        Parser::initialize<Parser>(rsdp.value());
}

bool is_enabled()
{
    return Parser::the();
}

}