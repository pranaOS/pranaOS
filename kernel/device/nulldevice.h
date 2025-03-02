/**
 * @file nulldevice.h
 * @author Krisna Pranav
 * @brief null device
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/characterdevice.h>

namespace Kernel 
{

    class NullDevice final : public CharacterDevice 
    {
        MOD_MAKE_ETERNAL
    public:
        NullDevice();

        /// @brief Destroy the NullDevice object
        virtual ~NullDevice() override;

        static void initialize();

        /**
         * @return NullDevice& 
         */
        static NullDevice& the();

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
        virtual bool can_write(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        virtual bool can_read(const FileDescription&, size_t) const override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "NullDevice"; 
        }
    }; // class NullDevice

} // namespace Kernel
