/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/devices/Device.h"

struct Private : public Device
{
private:
    RefPtr<Device> _disk;
    int _index;
    size64_t _start;
    size64_t _size;

    size64_t end() { return _start + _size; }

public:
    Partition(RefPtr<Device> disk, int index, size64_t start, size64_t size)
        : Device({}, DeviceClass::PARTITION),
          _disk{disk},
          _index{index},
          _start{start},
          _size{size}
    {
    }

    ~Partition()
    {
    }

    bool can_read() override
    {
        return _disk->can_read();
    }

    bool can_read() override
    {
        return _disk->can_write();
    }

    size_t size() override { return _size; }

};