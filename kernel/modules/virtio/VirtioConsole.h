/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "virtio/VirtioDevice.h"

struct VirtioConsole : public VirtioDevice
{
private:
public:
    VirtioConsole(DeviceAddress address) : VirtioDevice(address, DeviceClass::CONSOLE)
    {
    }

    ~VirtioConsole()
    {
    }
};