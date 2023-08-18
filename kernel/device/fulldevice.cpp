/**
 * @file fulldevice.cpp
 * @author Krisna Pranav
 * @brief full device
 * @version 6.0
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
    { }
    
    /// @brief Destroy the FullDevice::FullDevice object
    FullDevice::~FullDevice()
    { }
    
    /**
     * @return true 
     * @return false 
     */
    bool FullDevice::can_read(const FileDescription&, size_t) const
    {
        return true;
    }
    
    /**
     * @param buffer 
     * @param size 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> FullDevice::read(FileDescription&, size_t, UserOrKernelBuffer& buffer, size_t size)
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
    KResultOr<size_t> FullDevice::write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t size)
    {
        if (size == 0)
            return 0;
            
        return KResult(-ENOSPC);
    }

} // namespace Kernel
