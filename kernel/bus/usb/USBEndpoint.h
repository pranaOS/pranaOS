/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/bus/usb/USBDescriptors.h>
#include <kernel/bus/usb/USBPipe.h>

namespace Kernel::USB {

class USBEndpoint {
    static constexpr u8 ENDPOINT_ADDRESS_NUMBER_MASK = 0x0f;
    static constexpr u8 ENDPOINT_ADDRESS_DIRECTION_MASK = 0x80;

    static constexpr u8 ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_MASK = 0x03;
    static constexpr u8 ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_CONTROL = 0x00;
    static constexpr u8 ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_ISOCHRONOUS = 0x01;
    static constexpr u8 ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK = 0x02;
    static constexpr u8 ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT = 0x03;

    static constexpr u8 ENDPOINT_ATTRIBUTES_ISO_MODE_SYNC_TYPE = 0x0c;
    static constexpr u8 ENDPOINT_ATTRIBUTES_ISO_MODE_USAGE_TYPE = 0x30;

};

}