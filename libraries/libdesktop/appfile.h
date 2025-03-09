/**
 * @file appfile.h
 * @author Krisna Pranav
 * @brief App File
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <libcore/configfile.h>
#include <libgui/fileiconprovider.h>
#include <libgui/icon.h>

namespace Desktop 
{

    class AppFile : public RefCounted<AppFile> 
    {
    public:
        static constexpr char const* APP_FILES_DIRECTORY = "/res/apps";

        /**
         * @brief Get the for app object
         * 
         * @param app_name 
         * @return NonnullRefPtr<AppFile> 
         */
        static NonnullRefPtr<AppFile> get_for_app(StringView app_name);

        /**
         * @param path 
         * @return NonnullRefPtr<AppFile> 
         */
        static NonnullRefPtr<AppFile> open(StringView path);

        /**
         * @param directory 
         */
        static void for_each(Function<void(NonnullRefPtr<AppFile>)>, StringView directory = APP_FILES_DIRECTORY);

        /**
         * @brief Destroy the AppFile object
         * 
         */
        ~AppFile() = default;

        bool is_valid() const 
        { 
            return m_valid; 
        }

        /**
         * @return String 
         */
        String filename() const 
        { 
            return m_config->filename(); 
        }

        /**
         * @return String 
         */
        String name() const;
        String executable() const;
        String category() const;
        String description() const;
        String icon_path() const;
        GUI::Icon icon() const;
        bool run_in_terminal() const;
        Vector<String> launcher_file_types() const;
        Vector<String> launcher_protocols() const;
        bool spawn() const;

    private:
        /**
         * @brief Construct a new AppFile object
         * 
         * @param path 
         */
        explicit AppFile(StringView path);

        bool validate() const;

        RefPtr<Core::ConfigFile> m_config;
        bool m_valid { false };
    }; // class AppFile : public RefCounted<AppFile>

} // namespace Desktop
