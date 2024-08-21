/**
 * @file zerodevice.cpp
 * @author Krisna Pranav
 * @brief zero device
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/device/zerodevice.h>
#include <mods/memory.h>
#include <mods/stdlibextra.h>

namespace Kernel 
{
    
    /// @brief Construct a new ZeroDevice::ZeroDevice object
    ZeroDevice::ZeroDevice()
        : CharacterDevice(1, 5)
    { }
    
    /// @brief Destroy the ZeroDevice::ZeroDevice object
    ZeroDevice::~ZeroDevice()
    { }

    /**
     * @return true 
     * @return false 
     */
    bool ZeroDevice::can_read(const FileDescription&, size_t) const
    {
        return true;
    }

    /**
     * @param buffer 
     * @param size 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> ZeroDevice::read(FileDescription&, size_t, UserOrKernelBuffer& buffer, size_t size)
    {
        ssize_t count = min(static_cast<size_t>(PAGE_SIZE), size);

        if (!buffer.memset(0, count))
            return KResult(-EFAULT);

        return count;
    }

    /**
     * @param size 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> ZeroDevice::write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t size)
    {
        return min(static_cast<size_t>(PAGE_SIZE), size);
    }

} // namespace Kernel