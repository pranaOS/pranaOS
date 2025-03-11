/**
 * @file launcher.h
 * @author Krisna Pranav
 * @brief launcher
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/forward.h>
#include <mods/hashmap.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/refcounted.h>
#include <mods/string.h>

namespace Desktop 
{

    class Launcher 
    {
    public:

        enum class LauncherType {
            Default = 0,
            Application,
            UserPreferred,
            UserDefault
        }; // enum class LauncherType

        struct Details : public RefCounted<Details> {
            String name;
            String executable;
            LauncherType launcher_type { LauncherType::Default };

            /**
             * @return NonnullRefPtr<Details> 
             */
            static NonnullRefPtr<Details> from_details_str(String const&);
        }; // struct Details : public RefCounted<Details>

        static void ensure_connection();

        /**
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> add_allowed_url(URL const&);

        /**
         * @param handler 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> add_allowed_handler_with_any_url(String const& handler);

        /**
         * @param handler 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> add_allowed_handler_with_only_specific_urls(String const& handler, Vector<URL> const&);
        
        static ErrorOr<void> seal_allowlist();

        /**
         * @param handler_name 
         * @return true 
         * @return false 
         */
        static bool open(const URL&, String const& handler_name = {});

        /**
         * @param details 
         * @return true 
         * @return false 
         */
        static bool open(const URL&, Details const& details);

        static Vector<String> get_handlers_for_url(const URL&);

        /**
         * @brief Get the handlers with details for url object
         * 
         * @return NonnullRefPtrVector<Details> 
         */
        static NonnullRefPtrVector<Details> get_handlers_with_details_for_url(const URL&);
    }; // class Launcher

} // namespace Desktop
