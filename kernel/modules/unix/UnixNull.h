/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "unix/UNIXDevice.h"

struct UnixNull : public UNIXDevice
{

private:
public:
    UnixNull(DeviceAddress address) : UNIXDevice(address, DeviceClass::NULL_)
    {
    }

    ~UnixNull() {}

    ResultOr<size_t> read(size64_t offset, void *buffer, size_t size) override
    {
        UNUSED(offset);
        UNUSED(buffer);
        UNUSED(size);

        return 0;
    }
}