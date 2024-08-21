/**
 * @file zerodevice.h
 * @author Krisna Pranav
 * @brief zero device
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/characterdevice.h>

namespace Kernel 
{

    class ZeroDevice final : public CharacterDevice 
    {
        MOD_MAKE_ETERNAL

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

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;

        virtual bool can_write(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "ZeroDevice"; 
        }
    }; // class ZeroDevice

} // namespace Kernel
