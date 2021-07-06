/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/devices/Device.h"
#include "system/node/Handle.h"
#include "system/node/Node.h"

struct FsDevice : public FsNode
{
private:
    RefPtr<Device> _device;

public:
    FsDevice(RefPtr<Device> device)
        : FsNode(J_FILE_TYPE_DEVICE),
          _device(device)
    {
    }

    ~FsDevice() 
    {
    }

    size_t size() override
    {
        return _device->size();
    }

    bool can_read(FsHandle &) override
    {
        return _device->can_read();
    }

    bool can_write(FsHandle &) override
    {
        return _device->can_write();
    }
}