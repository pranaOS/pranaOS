/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/bus/pci/Device.h>

namespace Kernel {
namespace PCI {

Device::Device(Address address)
    : IRQHandler(get_interrupt_line(address))
    , m_pci_address(address)
{

}

Device::Device(Address address, u8 interrupt_vector)
    : IRQHandler(interrupt_vector)
    , m_pci_address(address)
{
}

Device::~Device()
{
}

}
}
