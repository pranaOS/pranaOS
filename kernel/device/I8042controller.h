/**
 * @file I8042controller.h
 * @author Krisna Pranav
 * @brief i8042 controller
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/spinlock.h>

namespace Kernel 
{

    #define I8042_BUFFER 0x60
    #define I8042_STATUS 0x64
    #define I8042_ACK 0xFA
    #define I8042_RESEND 0xFE
    #define I8042_BUFFER_FULL 0x01
    #define I8042_WHICH_BUFFER 0x20

    /// @brief: KEYBOARD + MOUSE BUFER
    #define I8042_KEYBOARD_BUFFER 0x00
    #define I8042_MOUSE_BUFFER 0x20

    class I8042Device 
    {
    public:

        /// @brief Destroy the I8042Device object
        virtual ~I8042Device() { }

        /**
         * @param byte 
         */
        virtual void irq_handle_byte_read(u8 byte) = 0;

        /// @brief enable interrupts
        virtual void enable_interrupts() = 0;
    };

    class I8042Controller 
    {
    public:
        enum class Device 
        {
            None = 0,
            Keyboard,
            Mouse
        }; // enum

        static void initialize();

        /**
         * @return I8042Controller& 
         */
        static I8042Controller& the();

        /**
         * @param device 
         * @return true 
         * @return false 
         */
        bool reset_device(Device device)
        {
            ScopedSpinLock lock(m_lock);

            return do_reset_device(device);
        }

        /**
         * @param device 
         * @param command 
         * @return u8 
         */
        u8 send_command(Device device, u8 command)
        {
            ScopedSpinLock lock(m_lock);
            return do_send_command(device, command);
        }

        /**
         * @param device 
         * @param command 
         * @param data 
         * @return u8 
         */
        u8 send_command(Device device, u8 command, u8 data)
        {
            ScopedSpinLock lock(m_lock);
            return do_send_command(device, command, data);
        }

        /**
         * @param device 
         * @return u8 
         */
        u8 read_from_device(Device device)
        {
            ScopedSpinLock lock(m_lock);
            return do_read_from_device(device);
        }

        /**
         * @param port 
         * @param data 
         */
        void wait_then_write(u8 port, u8 data)
        {
            ScopedSpinLock lock(m_lock);
            do_wait_then_write(port, data);
        }

        /**
         * @param port 
         * @return u8 
         */
        u8 wait_then_read(u8 port)
        {
            ScopedSpinLock lock(m_lock);
            return do_wait_then_read(port);
        }
        
        /// @brief: prepare_for_output
        void prepare_for_output();

        /// @brief: prepare_for_input
        void prepare_for_input(Device);

        /// @breif: irq_process_input_buffer
        void irq_process_input_buffer(Device);

    private:
        /// @brief Construct a new I8042Controller object
        I8042Controller();

        /// @brief: do_drain 
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
        }; // struct DeviceInfo

        SpinLock<u8> m_lock;

        bool m_is_dual_channel { false };

        DeviceInfo m_devices[2];
    }; // class I8042Controller

} // namespace Kernel
