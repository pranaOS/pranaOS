/**
 * @file nulldevice.cpp
 * @author Krisna Pranav
 * @brief null device
 * @version 6.0
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

    /// @brief: initialize
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
    
    /// @brief Construct a new NullDevice::NullDevice object
    NullDevice::NullDevice()
        : CharacterDevice(1, 3)
    { }

    /// @brief Destroy the NullDevice::NullDevice object
    NullDevice::~NullDevice()
    { }
    
    /**
     * @return true 
     * @return false 
     */
    bool NullDevice::can_read(const FileDescription&, size_t) const
    {
        return true;
    }
    
    /**
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> NullDevice::read(FileDescription&, size_t, UserOrKernelBuffer&, size_t)
    {
        return 0;
    }
    
    /**
     * @param buffer_size 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> NullDevice::write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t buffer_size)
    {
        return min(static_cast<size_t>(PAGE_SIZE), buffer_size);
    }

} // namespace Kernel
