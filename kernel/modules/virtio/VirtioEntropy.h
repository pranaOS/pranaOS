/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "virtio/VirtioDevice.h"

struct VirtioEntropy : public VirtioDevice
{
private:
public:
    VirtioEntropy(DeviceAddress address) : VirtioDevice(address, DeviceClass::RANDOM)
    {
    }

    ~VirtioEntropy()
    {
    }
};