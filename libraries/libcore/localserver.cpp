/**
 * @file localserver.cpp
 * @author Krisna Pranav
 * @brief local server
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/localserver.h>
#include <libcore/notifier.h>
#include <libcore/stream.h>
#include <libcore/system.h>
#include <libcore/systemservertakeover.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef SOCK_NONBLOCK
#    include <sys/ioctl.h>
#endif

namespace Core
{
    /**
     * @brief Construct a new LocalServer::LocalServer object
     * 
     * @param parent 
     */
    LocalServer::LocalServer(Object* parent)
        : Object(parent)
    {}  


    /**
     * @brief Destroy the LocalServer::LocalServer object
     * 
     */
    LocalServer::~LocalServer()
    {
        if (m_fd >= 0)
            ::close(m_fd);
    }

    /**
     * @param socket_path 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> LocalServer::take_over_from_system_server(String const& socket_path)
    {
        if (m_listening)
            return Error::from_string_literal("Core::LocalServer: Can't perform socket takeover when already listening"sv);

        auto socket = TRY(take_over_socket_from_system_server(socket_path));
        m_fd = TRY(socket->release_fd());

        m_listening = true;
        setup_notifier();
        return {};
    }

    void LocalServer::setup_notifier()
    {
        m_notifier = Notifier::construct(m_fd, Notifier::Event::Read, this);
        
        m_notifier->on_ready_to_read = [this] {
            if (on_accept) {
                auto maybe_client_socket = accept();
                if (maybe_client_socket.is_error()) {
                    dbgln("LocalServer::on_ready_to_read: Error accepting a connection: {} (FIXME: should propagate!)", maybe_client_socket.error());
                    return;
                }

                on_accept(maybe_client_socket.release_value());
            }
        };
    }

} // namespace Core
