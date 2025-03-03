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
    
} // namespace Core