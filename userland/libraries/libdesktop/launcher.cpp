/**
 * @file launcher.cpp
 * @author Krisna Pranav
 * @brief launcher
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/jsonobject.h>
#include <mods/url.h>
#include <launchserver/launchclientendpoint.h>
#include <launchserver/launchserverendpoint.h>
#include <libdesktop/launcher.h>
#include <libipc/connectiontoserver.h>
#include <stdlib.h>

namespace Desktop 
{

    /**
     * @param details_str 
     * @return NonnullRefPtr<Details> 
     */
    auto Launcher::Details::from_details_str(String const& details_str) -> NonnullRefPtr<Details>
    {
        auto details = adopt_ref(*new Details);
        auto json = JsonValue::from_string(details_str).release_value_but_fixme_should_propagate_errors();
        auto const& obj = json.as_object();
        details->executable = obj.get("executable").to_string();
        details->name = obj.get("name").to_string();
        if (auto type_value = obj.get_ptr("type")) {
            auto type_str = type_value->to_string();
            if (type_str == "app")
                details->launcher_type = LauncherType::Application;
            else if (type_str == "userpreferred")
                details->launcher_type = LauncherType::UserPreferred;
            else if (type_str == "userdefault")
                details->launcher_type = LauncherType::UserDefault;
        }
        return details;
    }

    class ConnectionToLaunchServer final
        : public IPC::ConnectionToServer<LaunchClientEndpoint, LaunchServerEndpoint>
        , public LaunchClientEndpoint {
        IPC_CLIENT_CONNECTION(ConnectionToLaunchServer, "/tmp/portal/launch")
    private:
        /**
         * @brief Construct a new Connection To Launch Server object
         * 
         * @param socket 
         */
        ConnectionToLaunchServer(NonnullOwnPtr<Core::Stream::LocalSocket> socket)
            : IPC::ConnectionToServer<LaunchClientEndpoint, LaunchServerEndpoint>(*this, move(socket))
        {
        }
    }; // class ConnectionToLaunchServer

    static ConnectionToLaunchServer& connection()
    {
        static auto connection = ConnectionToLaunchServer::try_create().release_value_but_fixme_should_propagate_errors();
        return connection;
    }

    void Launcher::ensure_connection()
    {
        [[maybe_unused]] auto& conn = connection();
    }

    /**
     * @param url 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Launcher::add_allowed_url(URL const& url)
    {
        auto response_or_error = connection().try_add_allowed_url(url);
        if (response_or_error.is_error())
            return Error::from_string_literal("Launcher::add_allowed_url: Failed"sv);
        return {};
    }

    /**
     * @param handler 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Launcher::add_allowed_handler_with_any_url(String const& handler)
    {
        auto response_or_error = connection().try_add_allowed_handler_with_any_url(handler);
        if (response_or_error.is_error())
            return Error::from_string_literal("Launcher::add_allowed_handler_with_any_url: Failed"sv);
        return {};
    }

    /**
     * @param handler 
     * @param urls 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Launcher::add_allowed_handler_with_only_specific_urls(String const& handler, Vector<URL> const& urls)
    {
        auto response_or_error = connection().try_add_allowed_handler_with_only_specific_urls(handler, urls);
        if (response_or_error.is_error())
            return Error::from_string_literal("Launcher::add_allowed_handler_with_only_specific_urls: Failed"sv);
        return {};
    }

    ErrorOr<void> Launcher::seal_allowlist()
    {
        auto response_or_error = connection().try_seal_allowlist();
        if (response_or_error.is_error())
            return Error::from_string_literal("Launcher::seal_allowlist: Failed"sv);
        return {};
    }

    /**
     * @param url 
     * @param handler_name 
     * @return true 
     * @return false 
     */
    bool Launcher::open(const URL& url, String const& handler_name)
    {
        return connection().open_url(url, handler_name);
    }

    /**
     * @param url 
     * @param details 
     * @return true 
     * @return false 
     */
    bool Launcher::open(const URL& url, Details const& details)
    {
        VERIFY(details.launcher_type != LauncherType::Application); 
        return open(url, details.executable);
    }

    /**
     * @param url 
     * @return Vector<String> 
     */
    Vector<String> Launcher::get_handlers_for_url(const URL& url)
    {
        return connection().get_handlers_for_url(url.to_string());
    }

    /**
     * @param url 
     * @return NonnullRefPtrVector<Details> 
     */
    auto Launcher::get_handlers_with_details_for_url(const URL& url) -> NonnullRefPtrVector<Details>
    {
        auto details = connection().get_handlers_with_details_for_url(url.to_string());
        NonnullRefPtrVector<Details> handlers_with_details;
        for (auto& value : details) {
            handlers_with_details.append(Details::from_details_str(value));
        }
        return handlers_with_details;
    }

}
