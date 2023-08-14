/**
 * @file zerodevice.h
 * @author Krisna Pranav
 * @brief zero device
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/characterdevice.h>

namespace Kernel
{
    class ZeroDevice final : public CharacterDevice
    {
    
    public:
        /// @brief Construct a new ZeroDevice object
        ZeroDevice();

        /// @brief Destroy the ZeroDevice object
        virtual ~ZeroDevice() override;

    private:
        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;
    }; // class ZeroDevice
    
} // namespace Kernel