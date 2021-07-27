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

public:
    const USBEndpointDescriptor& descriptor() const { return m_descriptor; }

    bool is_control() const { return (m_descriptor.endpoint_attributes_bitmap & ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_MASK) == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_CONTROL; }
    bool is_isochronous() const { return (m_descriptor.endpoint_attributes_bitmap & ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_MASK) == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_ISOCHRONOUS; }
    bool is_bulk() const { return (m_descriptor.endpoint_attributes_bitmap & ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_MASK) == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK; }
    bool is_interrupt() const { return (m_descriptor.endpoint_attributes_bitmap & ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_MASK) == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT; }

    u16 max_packet_size() const { return m_descriptor.max_packet_size; }
    u8 polling_interval() const { return m_descriptor.poll_interval_in_frames; }

private:
    USBEndpoint();
    USBEndpointDescriptor m_descriptor;

    USBPipe m_pipe;
};

}