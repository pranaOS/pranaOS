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


}