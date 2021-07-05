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

Iteration pci_scan_slot(IterFunc<PCIAddress> &callback, int bus, int slot)
{
    PCIAddress address{bus, slot, 0};

    if (address.read16(PCI_VENDOR_ID) == PCI_NONE)
    {
        return Iteration::CONTINUE;
    }

    if (pci_scan_func(callback, address) == Iteration::STOP)
    {
        return Iteration::STOP;
    }

    if (!address.read8(PCI_HEADER_TYPE))
    {
        return Iteration::CONTINUE;
    }

    for (int func = 1; func < 8; func++)
    {
        PCIAddress address{bus, slot, func};

        if (address.read16(PCI_VENDOR_ID) != PCI_NONE &&
            pci_scan_func(callback, address) == Iteration::STOP)
        {
            return Iteration::STOP;
        }
    }

    return Iteration::CONTINUE;
}

Iteration pci_scan_bus(IterFunc<PCIAddress> &callback, int bus)
{
    for (int slot = 0; slot < 32; ++slot)
    {
        if (pci_scan_slot(callback, bus, slot) == Iteration::STOP)
        {
            return Iteration::STOP;
        }
    }

    return Iteration::CONTINUE;
}

Iteration pci_scan(IterFunc<PCIAddress> callback)
{
    PCIAddress address{};

    if ((address.read8(PCI_HEADER_TYPE) & 0x80) == 0)
    {
        return pci_scan_bus(callback, 0);
    }

    for (int func = 0; func < 8; ++func)
    {
        PCIAddress address{0, 0, func};

        if (address.read16(PCI_VENDOR_ID) == PCI_NONE)
        {
            return Iteration::CONTINUE;
        }

        if (pci_scan_bus(callback, func) == Iteration::STOP)
        {
            return Iteration::STOP;
        }
    }

    return Iteration::CONTINUE;
}

static bool _has_isa_bridge = false;
static PCIAddress _isa_bridge_address = {};
static uint8_t _isa_remaps[4] = {};

void pci_initialize_isa_bridge()
{
    auto is_isa_bridge = [](PCIAddress &address) {
        return address.read16(PCI_CLASS) == 0x06 &&
               address.read16(PCI_SUBCLASS) == 0x01;
    };

    pci_scan([&](PCIAddress address) {
        if (is_isa_bridge(address))
        {
            Kernel::logln("Found isa bridge on PCI:{02x}:{02x}.{x}", address.bus(), address.slot(), address.func());
            _isa_bridge_address = address;
            _has_isa_bridge = true;

            for (int i = 0; i < 4; ++i)
            {
                _isa_remaps[i] = address.read8(0x60 + i);

                if (_isa_remaps[i] == 0x80)
                {
                    _isa_remaps[i] = 10 + (i % 1);
                }
            }

            uint32_t out = 0;
            memcpy(&out, &_isa_remaps, 4);
            address.write32(0x60, out);

            return Iteration::STOP;
        }
        else
        {
            return Iteration::CONTINUE;
        }
    });
}