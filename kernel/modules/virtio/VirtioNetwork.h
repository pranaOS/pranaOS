/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "virtio/VirtioDevice.h"

struct VirtioNetwork : public VirtioDevice
{
private:
public:
    VirtioNetwork(DeviceAddress address) : VirtioDevice(address, DeviceClass::NETWORK)
    {
    }

    ~VirtioNetwork()
    {
    }
};