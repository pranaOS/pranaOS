/**
 * @file socksproxyclient.cpp
 * @author Krisna Pranav
 * @brief socks proxy client
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/memorystream.h>
#include <libcore/socksproxyclient.h>

enum class Method : u8 
{

}; // enum class Method : u8 

enum class AddressType : u8 
{

}; // enum class AddressType : u8 

enum class Reply 
{

}; // enum class Reply 

struct [[gnu::packed]] Socks5VersionIdentifierAndMethodSelectionMessage
{

}; // struct [[gnu::packed]] Socks5VersionIdentifierAndMethodSelectionMessage

struct [[gnu::packed]] Socks5InitialResponse 
{

}; // struct [[gnu::packed]] Socks5InitialResponse 

struct [[gnu::packed]] Socks5ConnectRequestHeader 
{

}; // struct [[gnu::packed]] Socks5ConnectRequestHeader 

struct [[gnu::packed]] Socks5ConnectRequestTrailer
{
    
}; // struct [[gnu::packed]] Socks5ConnectRequestTrailer

namespace
{
    /**
     * @param reply 
     * @return StringView 
     */
    StringView reply_response_name(Reply reply)
    {
        switch (reply) {
        case Reply::Succeeded:
            return "Succeeded";
        
        }

        VERIFY_NOT_REACHED();
    }

    /**
     * @param socket 
     * @param version 
     * @param method 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> send_version_identifier_and_method_selection_message(Core::Stream::Socket& socket, Core::SOCKSProxyClient::Version version, Method method)
    {
        Socks5VersionIdentifierAndMethodSelectionMessage message {
            .version_identifier = to_underlying(version),
            .method_count = 1,
            .methods = { to_underlying(method) },
        }

        auto size = TRY(socket.write({ &message, sizeof(message) }));

        if (size != sizeof(message))
            return Error::from_string_literal("SOCKS5 negotiation failed");

        Socks5InitialResponse response;
        
        size = TRY(socket.read({ &response, sizeof(response) })).size();

        if (size != sizeof(response))
            return Error::from_string_literal("SOCKS5 negotiation failed");
        
    }
} // namespace

namespace Core 
{

    /**
     * @brief Destroy the SOCKSProxyClient::SOCKSProxyClient object
     * 
     */
    SOCKSProxyClient::~SOCKSProxyClient()
    {
        close();
        m_socket.on_ready_to_read = nullptr;
    }

    /**
     * @param underlying 
     * @param version 
     * @param target 
     * @param target_port 
     * @param auth_data 
     * @param command 
     * @return ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> 
     */
    ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> SOCKSProxyClient::connect(Socket& underlying, Version version, HostOrIPV4 const& target, int target_port, Variant<UsernamePasswordAuthenticationData, Empty> const& auth_data, Command command)
    {
        if (version != Version::V5)
            return Error::from_string_literal("SOCKS version not supported");

        return auth_data.visit(
            [&](Empty) -> ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> {
                TRY(send_version_identifier_and_method_selection_message(underlying, version, Method::NoAuth));
                auto reply = TRY(send_connect_request_message(underlying, version, target, target_port, command));
                if (reply != Reply::Succeeded) {
                    underlying.close();
                    return Error::from_string_literal(reply_response_name(reply));
                }

                return adopt_nonnull_own_or_enomem(new SOCKSProxyClient {
                    underlying,
                    nullptr,
                });
            },
            [&](UsernamePasswordAuthenticationData const& auth_data) -> ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> {
                TRY(send_version_identifier_and_method_selection_message(underlying, version, Method::UsernamePassword));
                auto auth_response = TRY(send_username_password_authentication_message(underlying, auth_data));
                if (auth_response != 0) {
                    underlying.close();
                    return Error::from_string_literal("SOCKS authentication failed");
                }

                auto reply = TRY(send_connect_request_message(underlying, version, target, target_port, command));
                if (reply != Reply::Succeeded) {
                    underlying.close();
                    return Error::from_string_literal(reply_response_name(reply));
                }

                return adopt_nonnull_own_or_enomem(new SOCKSProxyClient {
                    underlying,
                    nullptr,
                });
            });
    }

    /**
     * @param server 
     * @param server_port 
     * @param version 
     * @param target 
     * @param target_port 
     * @param auth_data 
     * @param command 
     * @return ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> 
     */
    ErrorOr<NonnullOwnPtr<SOCKSProxyClient>> SOCKSProxyClient::connect(HostOrIPV4 const& server, int server_port, Version version, HostOrIPV4 const& target, int target_port, Variant<UsernamePasswordAuthenticationData, Empty> const& auth_data, Command command)
    {
        auto underlying = TRY(server.visit(
            [&](u32 ipv4) {
                return Core::Stream::TCPSocket::connect({ IPv4Address(ipv4), static_cast<u16>(server_port) });
            },
            [&](String const& hostname) {
                return Core::Stream::TCPSocket::connect(hostname, static_cast<u16>(server_port));
            }));

        auto socket = TRY(connect(*underlying, version, target, target_port, auth_data, command));
        socket->m_own_underlying_socket = move(underlying);
        dbgln("SOCKS proxy connected, have {} available bytes", TRY(socket->m_socket.pending_bytes()));

        return socket;
    }

} // namespace Core
