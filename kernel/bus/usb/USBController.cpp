/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/bus/usb/USBController.h>

namespace Kernel::USB {

u8 USBController::allocate_address()
{
    return m_next_device_index++;
}

}
