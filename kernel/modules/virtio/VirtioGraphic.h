/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include "virtio/VirtioDevice.h"

struct VirtioGraphic : public VirtioDevice
{
private:
public:
    VirtioGraphic(DeviceAddress address) : VirtioDevice(address, DeviceClass::FRAMEBUFFER)
    {
    }

    ~VirtioGraphic()
    {
    }
};