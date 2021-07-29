/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/RandomDevice.h>
#include <kernel/Random.h>
#include <kernel/Sections.h>

namespace Kernel {

UNMAP_AFTER_INIT NonnullRefPtr<RandomDevice> RandomDevice::must_create()
{
    return adopt_ref(*new RandomDevice);
}

UNMAP_AFTER_INIT RandomDevice::RandomDevice()
    : CharacterDevice(1, 8)
{
}

UNMAP_AFTER_INIT RandomDevice::~RandomDevice()
{
}

bool RandomDevice::can_read(const FileDescription&, size_t) const
{
    return true;
}

KResultOr<size_t> RandomDevice::read(FileDescription&, u64, UserOrKernelBuffer& buffer, size_t size)
{
    return buffer.write_buffered<256>(size, [&](u8* data, size_t data_size) {
        get_good_random_bytes(data, data_size);
        return data_size;
    });
}

KResultOr<size_t> RandomDevice::write(FileDescription&, u64, const UserOrKernelBuffer&, size_t size)
{
    return size;
}

}