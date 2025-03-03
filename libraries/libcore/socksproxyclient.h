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
        enum class Version : u8 {
            V4 = 0x04,
            V5 = 0x05,
        }; // enum class Version : u8 

        struct UsernamePasswordAuthenticationData {
            String username;
            String password;
        }; // struct UsernamePasswordAuthenticationData

        enum class Command : u8 {
            Connect = 0x01,
            Bind = 0x02,
            UDPAssociate = 0x03,
        }; // enum class Command : u8

        using HostOrIPV4 = Variant<String, u32>;

        /**
         * @param underlying 
         * @param target 
         * @param target_port 
         * @param auth_data 
         * @return ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> 
         */
        static ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> connect(Socket& underlying, Version, HostOrIPV4 const& target, int target_port, Variant<UsernamePasswordAuthenticationData, Empty> const& auth_data = {}, Command = Command::Connect);

        /**
         * @param server 
         * @param server_port 
         * @param target 
         * @param target_port 
         * @param auth_data 
         * @return ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> 
         */
        static ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> connect(HostOrIPV4 const& server, int server_port, Version, HostOrIPV4 const& target, int target_port, Variant<UsernamePasswordAuthenticationData, Empty> const& auth_data = {}, Command = Command::Connect);

        /**
         * @brief Destroy the SOCKSProxyClient object
         * 
         */
        virtual ~SOCKSProxyClient() override;

        /**
         * @param bytes 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes bytes) override 
        { 
            return m_socket.read(bytes); 
        }

        /**
         * @param bytes 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes bytes) override 
        { 
            return m_socket.write(bytes); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_socket.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_socket.is_open(); 
        }

        virtual void close() override 
        { 
            m_socket.close(); 
        }

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> pending_bytes() const override 
        { 
            return m_socket.pending_bytes(); 
        }

        /**
         * @param timeout 
         * @return ErrorOr<bool> 
         */
        virtual ErrorOr<bool> can_read_without_blocking(int timeout = 0) const override 
        { 
            return m_socket.can_read_without_blocking(timeout); 
        }

        /**
         * @brief Set the blocking object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_blocking(bool enabled) override 
        { 
            return m_socket.set_blocking(enabled); 
        }

        /**
         * @brief Set the close on exec object
         * 
         * @param enabled 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> set_close_on_exec(bool enabled) override 
        { 
            return m_socket.set_close_on_exec(enabled); 
        }

        /**
         * @brief Set the notifications enabled object
         * 
         * @param enabled 
         */
        virtual void set_notifications_enabled(bool enabled) override 
        { 
            m_socket.set_notifications_enabled(enabled); 
        }

    private:
        /**
         * @brief Construct a new SOCKSProxyClient object
         * 
         * @param socket 
         * @param own_socket 
         */
        SOCKSProxyClient(Socket& socket, OwnPtr<Socket> own_socket)
            : m_socket(socket)
            , m_own_underlying_socket(move(own_socket))
        {
            m_socket.on_ready_to_read = [this] { on_ready_to_read(); };
        }

        Socket& m_socket;
        OwnPtr<Socket> m_own_underlying_socket;
    }; // class SOCKSProxyClient final : public Stream::Socket

} // namespace Core
