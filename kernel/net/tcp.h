/**
 * @file tcp.h
 * @author Krisna Pranav
 * @brief tcp
 * @version 6.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/net/ipv4.h>

namespace Kernel 
{

    struct TCPFlags 
    {
        enum : u16 {
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

        /**
         * @return u32 
         */
        u32 sequence_number() const 
        { 
            return m_sequence_number; 
        }

        /**
         * @param number 
         */
        void set_sequence_number(u32 number) 
        { 
            m_sequence_number = number; 
        }

        /**
         * @return u32 
         */
        u32 ack_number() const 
        { 
            return m_ack_number; 
        }

        /**
         * @param number 
         */
        void set_ack_number(u32 number) 
        { 
            m_ack_number = number; 
        }

        /**
         * @return u16 
         */
        u16 flags() const 
        { 
            return m_flags_and_data_offset & 0x1ff; 
        }

        /**
         * @param flags 
         */
        void set_flags(u16 flags) 
        { 
            m_flags_and_data_offset = (m_flags_and_data_offset & ~0x1ff) | (flags & 0x1ff); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_syn() const 
        { 
            return flags() & TCPFlags::SYN; 
        }

        bool has_ack() const 
        { 
            return flags() & TCPFlags::ACK; 
        }

        bool has_fin() const 
        { 
            return flags() & TCPFlags::FIN; 
        }

        bool has_rst() const 
        { 
            return flags() & TCPFlags::RST; 
        }

        /**
         * @return u8 
         */
        u8 data_offset() const 
        { 
            return (m_flags_and_data_offset & 0xf000) >> 12; 
        }

        /**
         * @param data_offset 
         */
        void set_data_offset(u16 data_offset) 
        { 
            m_flags_and_data_offset = (m_flags_and_data_offset & ~0xf000) | data_offset << 12; 
        }

        /**
         * @return u16 
         */
        u16 window_size() const 
        { 
            return m_window_size; 
        }

        /**
         * @param window_size 
         */
        void set_window_size(u16 window_size) 
        { 
            m_window_size = window_size; 
        }

        /**
         * @return u16 
         */
        u16 checksum() const 
        { 
            return m_checksum; 
        }

        /**
         * @param checksum 
         */
        void set_checksum(u16 checksum) 
        { 
            m_checksum = checksum; 
        }

        /**
         * @return u16 
         */
        u16 urgent() const 
        { 
            return m_urgent; 
        }

        /**
         * @param urgent 
         */
        void set_urgent(u16 urgent) 
        { 
            m_urgent = urgent; 
        }

        /**
         * @return const void* 
         */
        const void* payload() const 
        { 
            return ((const u8*)this) + header_size(); 
        }

        /**
         * @return void* 
         */
        void* payload() 
        { 
            return ((u8*)this) + header_size(); 
        }

    private:

        NetworkOrdered<u16> m_source_port;
        NetworkOrdered<u16> m_destination_port;

        NetworkOrdered<u32> m_sequence_number;
        NetworkOrdered<u32> m_ack_number;

        NetworkOrdered<u16> m_flags_and_data_offset;
        NetworkOrdered<u16> m_window_size;
        NetworkOrdered<u16> m_checksum;
        NetworkOrdered<u16> m_urgent;

    }; // class TCPPacket

    static_assert(sizeof(TCPPacket) == 20);

} // namespace Kernel
