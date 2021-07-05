/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Lock.h>
#include <libutils/RingBuffer.h>
#include "archs/x86/COM.h"
#include "ps2/LegacyDevice.h"


struct LegacySerial : public LegacyDevice
{

private:
    RingBuffer<char> _buffer{4096};
    Lock _buffer_lock{"legacy-serial"};

    COMPort port() { return (COMPort)legacy_address(); }


}