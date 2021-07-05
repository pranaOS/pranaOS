/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include "system/devices/Driver.h"

#include "ps2/Legacy.h"

struct LegacyDevice : public Device
{
private:
public:
    LegacyAddress legacy_address() { return address().legacy(); }

    LegacyDevice(DeviceAddress address, DeviceClass klass)
        : Device(address, klass)
    {
    }

    
};