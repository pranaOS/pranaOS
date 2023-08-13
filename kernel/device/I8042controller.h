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

    private:
        I8042Controller();

        /// @brief do_drain
        void do_drain();

        /**
         * @param device 
         * @return true 
         * @return false 
         */
        bool do_reset_device(Device device);

        /** 
         * @param device 
         * @param data 
         * @return u8 
         */
        u8 do_send_command(Device device, u8 data);

        /**
         * @param device 
         * @param command 
         * @param data 
         * @return u8 
         */
        u8 do_send_command(Device device, u8 command, u8 data);

        /**
         * @param device 
         * @param data 
         * @return u8 
         */
        u8 do_write_to_device(Device device, u8 data);

        /**
         * @param device 
         * @return u8 
         */
        u8 do_read_from_device(Device device);

        /**
         * @param port 
         * @param data 
         */
        void do_wait_then_write(u8 port, u8 data);

        /**
         * @param port 
         * @return u8 
         */
        u8 do_wait_then_read(u8 port);

        /**
         * @param device 
         * @return int 
         */
        static int device_to_deviceinfo_index(Device device)
        {
            ASSERT(device != Device::None);

            return (device == Device::Keyboard) ? 0 : 1;
        }

        struct DeviceInfo
        {
            I8042Device* device { nullptr };

            bool available { false };
        }

        SpinLock<u8> m_lock;

        bool m_is_dual_channel { false };

        DeviceInfo m_devices[2];

    }; // class I8042Controller

} // namespace Kernel