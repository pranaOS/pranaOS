/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/PCISerialDevice.h>
#include <kernel/Sections.h>

namespace Kernel {

static SerialDevice* s_the = nullptr;

SerialDevice& PCISerialDevice::the()
{
    VERIFY(s_the)
    return *s_the;
}

bool PCISerialDevice::is_available()
{
    return s_the;
}

}