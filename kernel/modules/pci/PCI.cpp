/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <libutils/Vector.h>
#include <string.h>

#include "archs/x86_32/x86_32.h"
#include "pci/PCI.h"

Iteration pci_scan_bus(IterFunc<PCIAddress> &callback, int bus);

Iteration pci_scan_hit(IterFunc<PCIAddress> &callback, PCIAddress &address)
{
    return callback(address);
}

Iteration pci_scan_func(IterFunc<PCIAddress> &callback, PCIAddress &address)
{
    if (pci_scan_hit(callback, address) == Iteration::STOP)
    {
        return Iteration::STOP;
    }

    if (address.read_class_sub_class() == PCI_TYPE_BRIDGE)
    {
        return pci_scan_bus(callback, address.read8(PCI_SECONDARY_BUS));
    }
    else
    {
        return Iteration::CONTINUE;
    }
}