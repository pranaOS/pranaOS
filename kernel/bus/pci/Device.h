/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/bus/pci/Definitions.h>
#include <kernel/interrupts/IRQHandler.h>

namespace Kernel {
class PCI::Device : public IRQHandler {
public:
    Address pci_address() const { return m_pci_address; }

protected:
    Device(Address pci_address);
    ~Device();

private:
    Address m_pci_address;
};

}