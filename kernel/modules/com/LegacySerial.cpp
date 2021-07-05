/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*//*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "com/LegacySerial.h"

LegacySerial::LegacySerial(DeviceAddress address) : LegacyDevice(address, DeviceClass::SERIAL)
{
    com_initialize(port());
}

void LegacySerial::handle_interrupt()
{
    LockHolder holder(_buffer_lock);

    uint8_t status = in8(port() + 2);

    if (status == 0b100)
    {
        char byte = com_getc(port());
        _buffer.put(byte);
    }
}
