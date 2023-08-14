/**
 * @file zerodevice.cpp
 * @author Krisna Pranav
 * @brief zero device
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "zerodevice.h"
#include "characterdevice.h"
#include <kernel/device/zerodevice.h>
#include <mods/memory.h>
#include <mods/stdlibextra.h>

namespace Kernel
{
    ZeroDevice::ZeroDevice()
        : CharacterDevice(1, 5)
    {}

    ZeroDevice::~ZeroDevice()
    {}

    bool ZeroDevice::can_read(const FileDescription&, size_t) const
    {
        return true;
    }
    
} // namespace kernel