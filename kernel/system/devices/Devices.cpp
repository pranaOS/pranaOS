/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libutils/Vector.h>
#include <skift/NumberFormatter.h>
#include <string.h>
#include "system/Streams.h"
#include "pci/PCI.h"
#include "ps2/Legacy.h"
#include "system/devices/Devices.h"
#include "system/devices/Driver.h"
#include "unix/UNIX.h"

static Vector<RefPtr<Device>> *_devices = nullptr;

static int _device_ids[(uint8_t)DeviceClass::__COUNT] = {};
static const char *_device_names[(uint8_t)DeviceClass::__COUNT] = {
#define DEVICE_NAMES_ENTRY(__type, __name) #__name,
    DEVICE_CLASS_LIST(DEVICE_NAMES_ENTRY)};

void device_scan(IterFunc<DeviceAddress> callback)
{
    if (legacy_scan([&](LegacyAddress address) {
            return callback(DeviceAddress(address));
        }) == Iteration::STOP)
    {
        return;
    }

    if (pci_scan([&](PCIAddress address) {
            return callback(DeviceAddress(address));
        }) == Iteration::STOP)
    {
        return;
    }

    if (unix_scan([&](UNIXAddress address) {
            return callback(DeviceAddress(address));
        }) == Iteration::STOP)
    {
        return;
    }
}

String device_claim_name(DeviceClass klass)
{
    _device_ids[(uint8_t)klass]++;

    if (_device_ids[(uint8_t)klass] == 1)
    {
        return _device_names[(uint8_t)klass];
    }
    else
    {
        return IO::format(
            "{}{}",
            _device_names[(uint8_t)klass],
            _device_ids[(uint8_t)klass] - 1);
    }
}

