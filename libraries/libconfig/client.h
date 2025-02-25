/**
 * @file client.h
 * @author Krisna Pranav
 * @brief client
 * @version 6.0
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <configserver/configclientendpoint.h>
#include <libcore/file.h>
#include <libcore/promise.h>
#include <libcore/standardpaths.h>
#include <libcip/connectiontoserver.h>

namespace Config 
{

    class Client 
    {
    private:
        explicit Client(NonnullOwnPtr<Core::Stream::LocalSocket> socket)
    }; // class Client


    /**
     * @param domain 
     * @param group 
     * @return Vector<String> 
     */
    inline Vector<String> list_keys(StringView domain, StringView group)
    {
        return Client::the().list_keys(domain, group);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return String 
     */
    inline String read_string(StringView domain, StringView group, StringView key, StringView fallback = {})
    {
        return Client::the().read_string(domain, group, key, fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return i32 
     */
    inline i32 read_i32(StringView domain, StringView group, StringView key, i32 fallback = 0)
    {
        return Client::the().read_i32(domain, group, key, fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return true 
     * @return false 
     */
    inline bool read_bool(StringView domain, StringView group, StringView key, bool fallback = false)
    {
        return Client::the().read_bool(domain, group, key, fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    inline void write_string(StringView domain, StringView group, StringView key, StringView value)
    {
        Client::the().write_string(domain, group, key, value);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    inline void write_i32(StringView domain, StringView group, StringView key, i32 value)
    {
        Client::the().write_i32(domain, group, key, value);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    inline void write_bool(StringView domain, StringView group, StringView key, bool value)
    {
        Client::the().write_bool(domain, group, key, value);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     */
    inline void remove_key(StringView domain, StringView group, StringView key)
    {
        Client::the().remove_key(domain, group, key);
    }

    /**
     * @param domains 
     */
    inline void pledge_domains(Vector<String> const& domains)
    {
        Client::the().pledge_domains(domains);
    }

    /**
     * @param domain 
     */
    inline void pledge_domain(String const& domain)
    {
        Client::the().pledge_domains({ domain });
    }

    /**
     * @param domain 
     */
    inline void monitor_domain(String const& domain)
    {
        Client::the().monitor_domain(domain);
    }


} // namespace Config 
