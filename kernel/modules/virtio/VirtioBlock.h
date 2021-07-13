/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "virtio/VirtioDevice.h"

struct VirtioBlock : public VirtioDevice
{
private:
public:
    VirtioBlock(DeviceAddress address) : VirtioDevice(address, DeviceClass::DISK)
    {
    }

    ~VirtioBlock()
    {
    }
};