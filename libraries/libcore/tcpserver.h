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
        C_OBJECT_ABSTRACT(TcpServer)

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
