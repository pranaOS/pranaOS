/**
 * @file characterdevice.h
 * @author Krisna Pranav
 * @brief character device
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/device.h>

namespace Kernel 
{

    class CharacterDevice : public Device 
    {
    public:
        /// @brief Destroy the CharacterDevice object
        virtual ~CharacterDevice() override;

    protected:
        /**
         * @param major 
         * @param minor 
         */
        CharacterDevice(unsigned major, unsigned minor)
            : Device(major, minor)
        { }

    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_character_device() const final 
        { 
            return true; 
        }
    }; // class CharacterDevice

} // namespace Kernel
