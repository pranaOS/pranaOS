/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include "pci/PCIDevice.h"
#include "virtio/Virtio.h"

struct VirtioDevice : public PCIDevice
{
private:
public:
    VirtioDevice(DeviceAddress address, DeviceClass klass) : PCIDevice(address, klass)
    {
    }

    ~VirtioDevice()
    {
    }
};