/**
 * @file ipv4socket.h
 * @author Krisna Pranav
 * @brief ipv4 socket
 * @version 6.0
 * @date 2023-08-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashmap.h>
#include <mods/singlelinkedlist.h>
#include <kernel/doublebuffer.h>
#include <kernel/kbuffer.h>
#include <kernel/lock.h>
#include <kernel/net/ipv4.h>
#include <kernel/net/ipv4socket_tuple.h>
#include <kernel/net/socket.h>

namespace Kernel 
{

    class NetworkAdapter;
    class TCPPacket;
    class TCPSocket;

    class IPv4Socket : public Socket 
    {
    public:

        /**
         * @param type 
         * @param protocol 
         * @return KResultOr<NonnullRefPtr<Socket>> 
         */
        static KResultOr<NonnullRefPtr<Socket>> create(int type, int protocol);

        /// @brief Destroy the IPv4Socket object
        virtual ~IPv4Socket() override;

        /**
         * @return Lockable<HashTable<IPv4Socket*>>& 
         */
        static Lockable<HashTable<IPv4Socket*>>& all_sockets();

        /**
         * @return KResult 
         */
        virtual KResult close() override;
        virtual KResult bind(Userspace<const sockaddr*>, socklen_t) override;
        virtual KResult connect(FileDescription&, Userspace<const sockaddr*>, socklen_t, ShouldBlock = ShouldBlock::Yes) override;
        virtual KResult listen(size_t) override;

        /// @breif: get[local, peer] address
        virtual void get_local_address(sockaddr*, socklen_t*) override;
        virtual void get_peer_address(sockaddr*, socklen_t*) override;

        /// @brief: attach, detach
        virtual void attach(FileDescription&) override;
        virtual void detach(FileDescription&) override;

        /// @brief: can_read + can_write
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> sendto(FileDescription&, const UserOrKernelBuffer&, size_t, int, Userspace<const sockaddr*>, socklen_t) override;

        /**
         * @param flags 
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> recvfrom(FileDescription&, UserOrKernelBuffer&, size_t, int flags, Userspace<sockaddr*>, Userspace<socklen_t*>, timeval&) override;

        /**
         * @param level 
         * @param option 
         * @return KResult 
         */
        virtual KResult setsockopt(int level, int option, Userspace<const void*>, socklen_t) override;
        
        /**
         * @param level 
         * @param option 
         * @return KResult 
         */
        virtual KResult getsockopt(FileDescription&, int level, int option, Userspace<void*>, Userspace<socklen_t*>) override;

        /**
         * @param request 
         * @param arg 
         * @return int 
         */
        virtual int ioctl(FileDescription&, unsigned request, FlatPtr arg) override;

        /**
         * @param peer_address 
         * @param peer_port 
         * @return true 
         * @return false 
         */
        bool did_receive(const IPv4Address& peer_address, u16 peer_port, KBuffer&&, const timeval&);

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& local_address() const 
        { 
            return m_local_address; 
        }

        /**
         * @return u16 
         */
        u16 local_port() const 
        { 
            return m_local_port; 
        }

        /**
         * @param port 
         */
        void set_local_port(u16 port) 
        { 
            m_local_port = port; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_specific_local_address() 
        { 
            return m_local_address.to_u32() != 0; 
        }

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& peer_address() const 
        { 
            return m_peer_address; 
        }
        
        /**
         * @return u16 
         */
        u16 peer_port() const 
        { 
            return m_peer_port; 
        }

        /**
         * @param port 
         */
        void set_peer_port(u16 port) 
        { 
            m_peer_port = port; 
        }

        /**
         * @return IPv4SocketTuple 
         */
        IPv4SocketTuple tuple() const 
        { 
            return IPv4SocketTuple(m_local_address, m_local_port, m_peer_address, m_peer_port); 
        }

        /**
         * @param description 
         * @return String 
         */
        String absolute_path(const FileDescription& description) const override;

        /**
         * @return u8 
         */
        u8 ttl() const 
        { 
            return m_ttl; 
        }

        enum class BufferMode 
        {
            Packets,
            Bytes,
        }; // enum

        /** 
         * @return BufferMode 
         */
        BufferMode buffer_mode() const 
        { 
            return m_buffer_mode; 
        }

    protected:
        /**
         * @param type 
         * @param protocol 
         */
        IPv4Socket(int type, int protocol);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "IPv4Socket"; 
        }

        int allocate_local_port_if_needed();

        /**
         * @return KResult 
         */
        virtual KResult protocol_bind() 
        { 
            return KSuccess; 
        }
        
        virtual KResult protocol_listen() { return KSuccess; }
        virtual KResultOr<size_t> protocol_receive(const KBuffer&, UserOrKernelBuffer&, size_t, int) { return -ENOTIMPL; }
        virtual KResultOr<size_t> protocol_send(const UserOrKernelBuffer&, size_t) { return -ENOTIMPL; }
        virtual KResult protocol_connect(FileDescription&, ShouldBlock) { return KSuccess; }
        virtual int protocol_allocate_local_port() { return 0; }
        virtual bool protocol_is_disconnected() const { return false; }

        virtual void shut_down_for_reading() override;

        void set_local_address(IPv4Address address) { m_local_address = address; }
        void set_peer_address(IPv4Address address) { m_peer_address = address; }

    private:
        virtual bool is_ipv4() const override { return true; }

        KResultOr<size_t> receive_byte_buffered(FileDescription&, UserOrKernelBuffer& buffer, size_t buffer_length, int flags, Userspace<sockaddr*>, Userspace<socklen_t*>);
        KResultOr<size_t> receive_packet_buffered(FileDescription&, UserOrKernelBuffer& buffer, size_t buffer_length, int flags, Userspace<sockaddr*>, Userspace<socklen_t*>, timeval&);

        void set_can_read(bool);

        IPv4Address m_local_address;
        IPv4Address m_peer_address;

        struct ReceivedPacket {
            IPv4Address peer_address;
            u16 peer_port;
            timeval timestamp;
            Optional<KBuffer> data;
        };

        SinglyLinkedListWithCount<ReceivedPacket> m_receive_queue;

        DoubleBuffer m_receive_buffer;

        u16 m_local_port { 0 };
        u16 m_peer_port { 0 };

        u32 m_bytes_received { 0 };

        u8 m_ttl { 64 };

        bool m_can_read { false };

        BufferMode m_buffer_mode { BufferMode::Packets };

        Optional<KBuffer> m_scratch_buffer;
    };

}
