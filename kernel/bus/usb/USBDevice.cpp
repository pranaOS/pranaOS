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

Device::~Device()
{
}

}