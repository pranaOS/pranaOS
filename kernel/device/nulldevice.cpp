/**
 * @file nulldevice.cpp
 * @author Krisna Pranav
 * @brief null device
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/device/nulldevice.h>
#include <mods/singleton.h>
#include <mods/stdlibextra.h>

namespace Kernel
{
    static Mods::Singleton<NullDevice> s_the;

    /// @brief: initialzie
    void NullDevice::initialize()
    {
        s_the.ensure_instance();
    }

    /**
     * @return NullDevice& 
     */
    NullDevice& NullDevice::the()
    {
        return *s_the;
    }

    NullDevice::NullDevice()
        : CharacterDevice(1, 3)
    {}
}