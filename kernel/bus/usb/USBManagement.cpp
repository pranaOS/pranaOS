/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <kernel/bus/pci/Access.h>
#include <kernel/bus/usb/UHCIController.h>
#include <kernel/bus/usb/USBManagement.h>
#include <kernel/CommandLine.h>
#include <kernel/Sections.h>

namespace Kernel::USB {

static Singleton<USBManagement> s_the;

UNMAP_AFTER_INIT USBManagement::USBManagement()
{
    enumerate_controllers();
}

UNMAP_AFTER_INIT void USBManagement::enumerate_controllers()
{
    if (kernel_command_line().disable_usb())
        return;

    PCI::enumerate([this](PCI::Address const& address, PCI::ID) {
        if (!(PCI::get_class(address) == 0xc && PCI::get_subclass(address) == 0x3))
            return;
        if (PCI::get_programming_interface(address) == 0x0) {
            if (kernel_command_line().disable_uhci_controller())
                return;

            if (auto uhci_controller_or_error = UHCIController::try_to_initialize(address); !uhci_controller_or_error.is_error())
                m_controllers.append(uhci_controller_or_error.release_value());

            return;
        }

        if (PCI::get_programming_interface(address) == 0x10) {
            dmesgln("USBManagement: OHCI controller found at {} is not currently supported.", address);
            return;
        }

        if (PCI::get_programming_interface(address) == 0x20) {
            dmesgln("USBManagement: EHCI controller found at {} is not currently supported.", address);
            return;
        }

        if (PCI::get_programming_interface(address) == 0x30) {
            dmesgln("USBManagement: xHCI controller found at {} is not currently supported.", address);
            return;
        }

        dmesgln("USBManagement: Unknown/unsupported controller at {} with programming interface 0x{:02x}", address, PCI::get_programming_interface(address));
    });
}

bool USBManagement::initialized()
{
    return s_the.is_initialized();
}

UNMAP_AFTER_INIT void USBManagement::initialize()
{
    s_the.ensure_instance();
}

USBManagement& USBManagement::the()
{
    return *s_the;
}

}
