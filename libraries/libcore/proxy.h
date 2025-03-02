/**
 * @file proxy.h
 * @author Krisna Pranav
 * @brief proxy
 * @version 6.0
 * @date 2025-03-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <AK/Error.h>
#include <AK/IPv4Address.h>
#include <AK/Types.h>
#include <AK/URL.h>
#include <LibIPC/Forward.h>

namespace Core 
{

    struct ProxyData {

        enum Type 
        {
            Direct,
            SOCKS5,
        } type { Type::Direct };

        u32 host_ipv4 { 0 };
        int port { 0 };

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(ProxyData const& other) const = default;

        /**
         * @param url 
         * @return ErrorOr<ProxyData> 
         */
        static ErrorOr<ProxyData> parse_url(URL const& url)
        {
            if (!url.is_valid())
                return Error::from_string_literal("Invalid proxy URL");

            ProxyData proxy_data;

            if (url.scheme() != "socks5")
                return Error::from_string_literal("Unsupported proxy type");

            proxy_data.type = ProxyData::Type::SOCKS5;

            auto host_ipv4 = IPv4Address::from_string(url.host());

            if (!host_ipv4.has_value())
                return Error::from_string_literal("Invalid proxy host, must be an IPv4 address");

            proxy_data.host_ipv4 = host_ipv4->to_u32();

            auto port = url.port();

            if (!port.has_value())
                return Error::from_string_literal("Invalid proxy, must have a port");

            proxy_data.port = *port;

            return proxy_data;
        }
    }; // struct ProxyData
} // namespace Core

namespace IPC 
{
    /**
     * @return true 
     * @return false 
     */
    bool encode(Encoder&, Core::ProxyData const&);

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> decode(Decoder&, Core::ProxyData&);
} // namespace IPC
