/**
 * @file fulldevice.h
 * @author Krisna Pranav
 * @brief full device
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
    class FullDevice final : public CharacterDevice
    {
    public:
        /// @brief Destroy the FullDevice object
        virtual ~FullDevice() override;

    private:    
        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&) const override;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override
        {
            return "FullDevice";
        }
    }; // class FullDevice

} // namespace Kernel