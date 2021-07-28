/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <kernel/devices/Device.h>
#include <kernel/filesystem/InodeMetadata.h>

namespace Kernel {

void Device::for_each(Function<void(Device&)> callback)
{
    for (auto& entry : all_devices())
        callback(*entry.value);
}

Device* Device::get_device(unsigned major, unsigned minor)
{
    auto it = all_devices().find(encoded_device(major, minor));
    if (it == all_devices().end())
        return nullptr;
    return it->value;
}

String Device::absolute_path(const FileDescription&) const 
{
    return absolute_path();
}

string Device::absolute_path() const
{
    return String::formatted("dev/{}", device_name());
}

}