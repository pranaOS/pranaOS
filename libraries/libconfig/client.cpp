/**
 * @file client.cpp
 * @author Krisna Pranav
 * @brief client
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libconfig/client.h>
#include <libconfig/listener.h>

namespace Config 
{

    static RefPtr<Client> s_the = nullptr;

    /**
     * @return Client& 
     */
    Client& Client::the()
    {
        if (!s_the || !s_the->is_open()) {
            VERIFY(Core::EventLoop::has_been_instantiated());
            s_the = Client::try_create().release_value_but_fixme_should_propagate_errors();
        }

        return *s_the;
    }

    /**
     * @param domains 
     */
    void Client::pledge_domains(Vector<String> const& domains)
    {
        async_pledge_domains(domains);
    }   

    /**
     * @param domain 
     */
    void Client::monitor_domain(String const& domain)
    {
        async_monitor_domain(domain);
    }

    /**
     * @param domain 
     * @param group 
     * @return Vector<String> 
     */
    Vector<String> Client::list_keys(StringView domain, StringView group)
    {
        return list_config_keys(domain, group);
    }

    /**
     * @param domain 
     * @return Vector<String> 
     */
    Vector<String> Client::list_groups(StringView domain)
    {
        return list_config_groups(domain);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return String 
     */
    String Client::read_string(StringView domain, StringView group, StringView key, StringView fallback)
    {
        return read_string_value(domain, group, key).value_or(fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return i32 
     */
    i32 Client::read_i32(StringView domain, StringView group, StringView key, i32 fallback)
    {
        return read_i32_value(domain, group, key).value_or(fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param fallback 
     * @return true 
     * @return false 
     */
    bool Client::read_bool(StringView domain, StringView group, StringView key, bool fallback)
    {
        return read_bool_value(domain, group, key).value_or(fallback);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::write_string(StringView domain, StringView group, StringView key, StringView value)
    {
        write_string_value(domain, group, key, value);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::write_i32(StringView domain, StringView group, StringView key, i32 value)
    {
        write_i32_value(domain, group, key, value);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::write_bool(StringView domain, StringView group, StringView key, bool value)
    {
        write_bool_value(domain, group, key, value);
    }   

    /**
     * @param domain 
     * @param group 
     * @param key 
     */
    void Client::remove_key(StringView domain, StringView group, StringView key)
    {
        async_remove_key(domain, group, key);
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::notify_changed_string_value(String const& domain, String const& group, String const& key, String const& value)
    {
        Listener::for_each([&](auto& listener) {
            listener.config_string_did_change(domain, group, key, value);
        });
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::notify_changed_i32_value(String const& domain, String const& group, String const& key, i32 value)
    {
        Listener::for_each([&](auto& listener) {
            listener.config_i32_did_change(domain, group, key, value);
        });
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     * @param value 
     */
    void Client::notify_changed_bool_value(String const& domain, String const& group, String const& key, bool value)
    {
        Listener::for_each([&](auto& listener) {
            listener.config_bool_did_change(domain, group, key, value);
        });
    }

    /**
     * @param domain 
     * @param group 
     * @param key 
     */
    void Client::notify_removed_key(String const& domain, String const& group, String const& key)
    {
        Listener::for_each([&](auto& listener) {
            listener.config_key_was_removed(domain, group, key);
        });
    }

} // namespace Config
