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
#include <configserver/configserverendpoint.h>
#include <libcore/file.h>
#include <libcore/promise.h>
#include <libcore/standardpaths.h>
#include <libipc/connectiontoserver.h>

namespace Config 
{

    class Client final
        : public IPC::ConnectionToServer<ConfigClientEndpoint, ConfigServerEndpoint>
        , public ConfigClientEndpoint 
    {
        IPC_CLIENT_CONNECTION(Client, "/tmp/portal/config")

    public:
        void pledge_domains(Vector<String> const&);
        void monitor_domain(String const&);

        /**
         * @param domain 
         * @return Vector<String> 
         */
        Vector<String> list_groups(StringView domain);

        /**
         * @param domain 
         * @param group 
         * @return Vector<String> 
         */
        Vector<String> list_keys(StringView domain, StringView group);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param fallback 
         * @return String 
         */
        String read_string(StringView domain, StringView group, StringView key, StringView fallback);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param fallback 
         * @return i32 
         */
        i32 read_i32(StringView domain, StringView group, StringView key, i32 fallback);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param fallback 
         * @return true 
         * @return false 
         */
        bool read_bool(StringView domain, StringView group, StringView key, bool fallback);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void write_string(StringView domain, StringView group, StringView key, StringView value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void write_i32(StringView domain, StringView group, StringView key, i32 value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void write_bool(StringView domain, StringView group, StringView key, bool value);

        /**
         * @param domain 
         * @param group 
         * @param key 
         */
        void remove_key(StringView domain, StringView group, StringView key);

        /**
         * @return Client& 
         */
        static Client& the();

    private:
        /**
         * @brief Construct a new Client object
         * 
         * @param socket 
         */
        explicit Client(NonnullOwnPtr<Core::Stream::LocalSocket> socket)
            : IPC::ConnectionToServer<ConfigClientEndpoint, ConfigServerEndpoint>(*this, move(socket))
        {
        }

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void notify_changed_string_value(String const& domain, String const& group, String const& key, String const& value) override;

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void notify_changed_i32_value(String const& domain, String const& group, String const& key, i32 value) override;

        /**
         * @param domain 
         * @param group 
         * @param key 
         * @param value 
         */
        void notify_changed_bool_value(String const& domain, String const& group, String const& key, bool value) override;

        /**
         * @param domain 
         * @param group 
         * @param key 
         */
        void notify_removed_key(String const& domain, String const& group, String const& key) override;
    }; // class Client

    /**
     * @param domain 
     * @return Vector<String> 
     */
    inline Vector<String> list_groups(StringView domain)
    {
        return Client::the().list_groups(domain);
    }

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
