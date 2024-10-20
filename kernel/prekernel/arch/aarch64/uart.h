/**
 * @file uart.h
 * @author Krisna Pranav
 * @brief UART
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Prekernel 
{
    struct UARTRegisters;

    class UART 
    {
    public:
        /**
         * @return UART& 
         */
        static UART& the();

        /**
         * @param c 
         */
        void send(u32 c);
        
        u32 receive();

        /**
         * @param s 
         */
        void print_str(const char* s)
        {
            while (*s)
                send(*s++);
        }

        /**
         * @param n 
         */
        void print_num(u64 n)
        {
            char buf[21];
            int i = 0;

            do {
                buf[i++] = (n % 10) + '0';
                n /= 10;
            } while (n);

            for (i--; i >= 0; i--) {
                send(buf[i]);
            }
        }

        /**
         * @param n 
         */
        void print_hex(u64 n)
        {
            char buf[17];
            static const char* digits = "0123456789ABCDEF";
            int i = 0;

            do {
                buf[i++] = digits[n % 16];
                n /= 16;
            } while (n);

            send(static_cast<u32>('0'));
            send(static_cast<u32>('x'));

            buf[16] = '\0';

            for (i--; i >= 0; i--) {
                send(buf[i]);
            }
        }

    private:
        UART();

        /**
         * @brief Set the baud rate object
         * 
         * @param baud_rate 
         * @param uart_frequency_in_hz 
         */
        void set_baud_rate(int baud_rate, int uart_frequency_in_hz);
        void wait_until_we_can_send();
        void wait_until_we_can_receive();

        UARTRegisters volatile* m_registers;
    }; // class UART 
} // namespace Prekernel