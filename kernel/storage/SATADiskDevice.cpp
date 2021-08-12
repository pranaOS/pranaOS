/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/filesystem/FileDescription.h>
#include <kernel/storage/AHCIController.h>
#include <kernel/storage/IDEChannel.h>
#include <kernel/storage/SATADiskDevice.h>

namespace Kernel {

NonnullRefPtr<SATADiskDevice> SATADiskDevice::create(const AHCIController& controller, const AHCIPort& port, size_t sector_size, u64 max_addressable_block)
{
    return adopt_ref(*new SATADiskDevice(controller, port, sector_size, max_addressable_block));
}

SATADiskDevice::SATADiskDevice(const AHCIController& controller, const AHCIPort& port, size_t sector_size, u64 max_addressable_block)
    : StorageDevice(controller, sector_size, max_addressable_block)
    , m_port(port)
{
}

SATADiskDevice::~SATADiskDevice()
{
}

StringView SATADiskDevice::class_name() const
{
    return "SATADiskDevice";
}

void SATADiskDevice::start_request(AsyncBlockDeviceRequest& request)
{
    m_port->start_request(request);
}

String SATADiskDevice::device_name() const
{
    return String::formatted("hd{:c}", 'a' + minor());
}

}
