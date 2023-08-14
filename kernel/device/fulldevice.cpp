/**
 * @file fulldevice.cpp
 * @author Krisna Pranav
 * @brief full device
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/memory.h>
#include <mods/stdlibextra.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/device/fulldevice.h>
#include <libc/errno_codes.h>

namespace Kernel
{
    /// @brief Construct a new FullDevice::FullDevice object
    FullDevice::FullDevice()
        : CharacterDevice(1, 7)
    {}

    /// @brief Destroy the FullDevice::FullDevice object
    FullDevice::~FullDevice()
    {}

    /**
     * @return true 
     * @return false 
     */
    bool FullDevice::can_read(const FileDescription&, size_t)
    {
        return true;
    }

} // namespace Kernel