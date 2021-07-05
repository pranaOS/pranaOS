/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/devices/Device.h"
#include "system/devices/DeviceAddress.h"

struct DeviceMatcher
{
private:
    DeviceBus _bus;
    const char *_name;

public:
    DeviceBus bus() const { return _bus; }

    const char *name() const { return _name; }
}