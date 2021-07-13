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


};