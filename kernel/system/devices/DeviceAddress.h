/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>

#include <libutils/Iteration.h>
#include <stdio.h>
#include "pci/PCIAddress.h"
#include "ps2/LegacyAddress.h"
#include "unix/UNIXAddress.h"
#include "virtio/VirtioAddress.h"


enum DeviceBus
{
    BUS_NONE,

    BUS_UNIX,
    BUS_PCI,
    BUS_LEGACY
};

struct DeviceAddress
{
private:
    DeviceBus _bus;

    union
    {
        LegacyAddress _legacy;
        PCIAddress _pci;
        UNIXAddress _unix;
    };


}