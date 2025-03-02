/**
 * @file fulldevice.h
 * @author Krisna Pranav
 * @brief full device
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/characterdevice.h>

namespace Kernel 
{

    class FullDevice final : public CharacterDevice 
    {
        MOD_MAKE_ETERNAL

    public:

        /// @brief Construct a new FullDevice object
        FullDevice();

        /// @brief Destroy the FullDevice object
        virtual ~FullDevice() override;

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
            return "FullDevice"; 
        }

    }; // class FullDevice

} // namespace Kernel