/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/bus/pci/Device.h>
#include <kernel/bus/pci/IDs.h>
#include <kernel/devices/CharacterDevice.h>
#include <kernel/devices/SerialDevice.h>

namespace Kernel {

class PCISerialDevice {
    BASE_MAKE_ETERNAL
public:
    static void detect();
    static SerialDevice& the();
    static bool is_available();

private:
    struct BoardDefinition {
        PCI::ID device_id;
        StringView name;
        u32 port_count { 0 };
        u32 pci_bar { 0 };
        u32 first_offset { 0 };
        u32 port_size { 0 };
        SerialDevice::Baud baud_rate { SerialDevice::Baud::Baud38400 };
    };

}