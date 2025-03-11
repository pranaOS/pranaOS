/**
 * @file udpserver.h
 * @author Krisna Pranav
 * @brief udp server
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/forward.h>
#include <mods/function.h>
#include <libcore/forward.h>
#include <libcore/object.h>
#include <libcore/socketaddress.h>

namespace Core 
{

    class UDPServer : public Object 
    {
        C_OBJECT(UDPServer)
    public:
        /**
         * @brief Destroy the UDPServer object
         * 
         */
        virtual ~UDPServer() override;

        /**
         * @return true 
         * @return false 
         */
        bool is_bound() const 
        { 
            return m_bound; 
        }

        /**
         * @param address 
         * @param port 
         * @return true 
         * @return false 
         */
        bool bind(IPv4Address const& address, u16 port);

        /**
         * @param size 
         * @param from 
         * @return ByteBuffer 
         */
        ByteBuffer receive(size_t size, sockaddr_in& from);

        /**
         * @param size 
         * @return ByteBuffer 
         */
        ByteBuffer receive(size_t size)
        {
            struct sockaddr_in saddr;
            return receive(size, saddr);
        };

        /**
         * @param to 
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> send(ReadonlyBytes, sockaddr_in const& to);

        /**
         * @return Optional<IPv4Address> 
         */
        Optional<IPv4Address> local_address() const;
        Optional<u16> local_port() const;

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

        Function<void()> on_ready_to_receive;

    protected:
        /**
         * @brief Construct a new UDPServer object
         * 
         * @param parent 
         */
        explicit UDPServer(Object* parent = nullptr);

    private:
        int m_fd { -1 };
        bool m_bound { false };
        RefPtr<Notifier> m_notifier;
    }; // class UDPServer : public Object 

} // namespace Core
