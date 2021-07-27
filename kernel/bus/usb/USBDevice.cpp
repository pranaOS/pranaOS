/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/OwnPtr.h>
#include <base/Types.h>
#include <base/Vector.h>
#include <kernel/bus/usb/UHCIController.h>
#include <kernel/bus/usb/USBDescriptors.h>
#include <kernel/bus/usb/USBDevice.h>
#include <kernel/bus/usb/USBRequest.h>

static u32 s_next_usb_address = 1; 

namespace Kernel::USB {

KResultOr<NonnullRefPtr<Device>> Device::try_create(PortNumber port, DeviceSpeed speed)
{
    auto pipe_or_error = Pipe::try_create_pipe(Pipe::Type::Control, Pipe::Direction::Bidirectional, 0, 8, 0);
    if (pipe_or_error.is_error())
        return pipe_or_error.error();

    auto device = AK::try_create<Device>(port, speed, pipe_or_error.release_value());
    if (!device)
        return ENOMEM;

    auto enumerate_result = device->enumerate();
    if (enumerate_result.is_error())
        return enumerate_result;

    return device.release_nonnull();
}

Device::Device(PortNumber port, DeviceSpeed speed, NonnullOwnPtr<Pipe> default_pipe)
    : m_device_port(port)
    , m_device_speed(speed)
    , m_address(0)
    , m_default_pipe(move(default_pipe))
{
}

KResult Device::enumerate()
{
    USBDeviceDescriptor dev_descriptor {};

    auto transfer_length_or_error = m_default_pipe->control_transfer(USB_DEVICE_REQUEST_DEVICE_TO_HOST, USB_REQUEST_GET_DESCRIPTOR, 0x100, 0, 8, &dev_descriptor);

    if (transfer_length_or_error.is_error())
        return transfer_length_or_error.error();

    auto transfer_length = transfer_length_or_error.release_value();

    VERIFY(transfer_length > 0);

    VERIFY(dev_descriptor.descriptor_header.descriptor_type == DESCRIPTOR_TYPE_DEVICE);
    m_default_pipe->set_max_packet_size(dev_descriptor.max_packet_size);

    transfer_length_or_error = m_default_pipe->control_transfer(USB_DEVICE_REQUEST_DEVICE_TO_HOST, USB_REQUEST_GET_DESCRIPTOR, 0x100, 0, sizeof(USBDeviceDescriptor), &dev_descriptor);

    if (transfer_length_or_error.is_error())
        return transfer_length_or_error.error();

    transfer_length = transfer_length_or_error.release_value();

    VERIFY(transfer_length > 0);

    VERIFY(dev_descriptor.descriptor_header.descriptor_type == DESCRIPTOR_TYPE_DEVICE);

    if constexpr (USB_DEBUG) {
        dbgln("USB Device Descriptor for {:04x}:{:04x}", dev_descriptor.vendor_id, dev_descriptor.product_id);
        dbgln("Device Class: {:02x}", dev_descriptor.device_class);
        dbgln("Device Sub-Class: {:02x}", dev_descriptor.device_sub_class);
        dbgln("Device Protocol: {:02x}", dev_descriptor.device_protocol);
        dbgln("Max Packet Size: {:02x} bytes", dev_descriptor.max_packet_size);
        dbgln("Number of configurations: {:02x}", dev_descriptor.num_configurations);
    }

    transfer_length_or_error = m_default_pipe->control_transfer(USB_DEVICE_REQUEST_HOST_TO_DEVICE, USB_REQUEST_SET_ADDRESS, s_next_usb_address, 0, 0, nullptr);

    if (transfer_length_or_error.is_error())
        return transfer_length_or_error.error();

    transfer_length = transfer_length_or_error.release_value();

    VERIFY(transfer_length > 0);
    m_address = s_next_usb_address++;

    memcpy(&m_device_descriptor, &dev_descriptor, sizeof(USBDeviceDescriptor));
    return KSuccess;
}

Device::~Device()
{
}

}