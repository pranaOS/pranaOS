/**
 * @file I8042controller.h
 * @author Krisna Pranav
 * @brief i8042 controller
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/spinlock.h>

namespace Kernel
{

    class I8042Device
    {
    public:

        /// @brief Destroy the I8042Device object
        virtual ~I8042Device() {}

        /**
         * @param byte 
         */
        virtual void irq_handle_byte_read(u8 byte) = 0;

        /// @brief: enable_interrupts
        virtual void enable_interrupts() = 0;
    }; // class I8042Device

    class I8042Controller
    {
    public:
        enum class Device
        {
            None = 0,
            Keyboard,
            Mouse
        }; 

        static void initialize();

    }; // class I8042Controller

} // namespace Kernel