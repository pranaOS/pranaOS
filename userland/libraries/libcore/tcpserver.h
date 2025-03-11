/**
 * @file tcpserver.h
 * @author Krisna Pranav
 * @brief tcp server
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ipv4address.h>
#include <libcore/notifier.h>
#include <libcore/object.h>
#include <libcore/stream.h>

namespace Core 
{

    class TCPServer : public Object 
    {
        C_OBJECT_ABSTRACT(TCPServer)
    public:
        /**
         * @param parent 
         * @return ErrorOr<NonnullRefPtr<TCPServer>> 
         */
        static ErrorOr<NonnullRefPtr<TCPServer>> try_create(Object* parent = nullptr);

        /**
         * @brief Destroy the TCPServer object
         * 
         */
        virtual ~TCPServer() override;

        /**
         * @return true 
         * @return false 
         */
        bool is_listening() const 
        { 
            return m_listening; 
        }

        /**
         * @param address 
         * @param port 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> listen(IPv4Address const& address, u16 port);

        /**
         * @brief Set the blocking object
         * 
         * @param blocking 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> set_blocking(bool blocking);

        /**
         * @return ErrorOr<NonnullOwnPtr<Stream::TCPSocket>> 
         */
        ErrorOr<NonnullOwnPtr<Stream::TCPSocket>> accept();

        /**
         * @return Optional<IPv4Address> 
         */
        Optional<IPv4Address> local_address() const;
        Optional<u16> local_port() const;

        Function<void()> on_ready_to_accept;

    private:
        /**
         * @brief Construct a new TCPServer object
         * 
         * @param fd 
         * @param parent 
         */
        explicit TCPServer(int fd, Object* parent = nullptr);

        int m_fd { -1 };
        bool m_listening { false };
        RefPtr<Notifier> m_notifier;
    }; // class TCPServer : public Object

} // namespace Core
