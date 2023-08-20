/**
 * @file ioapic.cpp
 * @author Krisna Pranav
 * @brief ioapic
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/optional.h>
#include <mods/string_view.h>
#include <kernel/acpi/multiprocessorparser.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/apic.h>
#include <kernel/interrupt/ioapic.h>
#include <kernel/interrupt/interruptmanagement.h>
#include <kernel/vm/memorymanager.h>

#define IOAPIC_REDUCTION_ENTRY_OFFSET 0x10

namespace Kernel
{

    enum DeliveryMode
    {
        Normal = 0,
        LowPriority = 1,
        SMI = 2,
        NMI = 3,
        INIT = 4,
        External = 7
    };

    IOAPIC::IOAPIC(PhysicalAddress address, u32 gsi_base)
        : m_address(address)
        , m_gsi_base(gsi_base)
    {
        InterruptDisabler disabler;
        mask_all_redirection_entries();
    }

    void IOAPIC::initialize()
    {}

} // namespace Kernel