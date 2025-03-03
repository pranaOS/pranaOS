/**
 * @file socksproxyclient.h
 * @author Krisna Pranav
 * @brief socks proxy client
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <libcore/proxy.h>
#include <libcore/stream.h>

namespace Core
{

    class SOCKSProxyClient final : public Stream::Socket 
    {
    public:

    private:
        SOCKSProxyClient(Socket& sock, OwnPtr<Socket> own_socket)
            : m_socket(socket)
            , m_own_underlying_socket(move(own_socket))
        {
            m_socket.on_ready_to_read = [this] { on_ready_to_read(); }
        }

        Socket& m_socket;
        OwnPtr<Socket> m_own_underlying_socket;
    }; // class SOCKSProxyClient final : public Stream::Socket 

} // namespace Core