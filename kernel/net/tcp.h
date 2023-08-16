/**
 * @file tcp.h
 * @author Krisna Pranav
 * @brief tcp
 * @version 1.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/net/ipv4.h>

namespace Kernel
{
    struct TCPFlags
    {
        enum : u16
        {
            FIN = 0x01,
            SYN = 0x02,
            RST = 0x04,
            PUSH = 0x08,
            ACK = 0x10,
            URG = 0x20
        };
    }; // struct TCPFlags

    class [[gnu::packed]] TCPPacket
    {
    public:
        TCPPacket() = default;
        ~TCPPacket() = default;
        
        /**
         * @return size_t 
         */
        size_t header_size() const
        {
            return data_offset() * sizeof(u32);
        }

        /**
         * @return u16 
         */
        u16 source_port() const
        {
            return m_source_port;
        }

        /**
         * @param port 
         */
        void set_source_port(u16 port)
        {
            m_source_port = port;
        }

        /**
         * @return u16 
         */
        u16 destination_port() const
        {
            return m_destination_port;
        }

        /**
         * @param port 
         */
        void set_destination_port(u16 port)
        {
            m_destination_port = port;
        }

    private:
        NetworkOrdered<u16> m_source_port;
        NetworkOrdered<u16> m_destination_port;
        NetworkOrdered<u32> m_sequence_number;
        NetworkOrdered<u32> m_ack_number;
        NetworkOrdered<u16> m_flags_and_data_offset;
        NetworkOrdered<u16> m_window_size;
        NetworkOrdered<u16> m_checksum;
        NetworkOrdered<u16> m_urgen; 
    }; // class 

} // namespace Kernel