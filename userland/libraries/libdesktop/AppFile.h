/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libcore/ConfigFile.h>
#include <libgui/FileIconProvider.h>
#include <libgui/Icon.h>

namespace Desktop {

class AppFile : public RefCounted<AppFile> {
public:
    static constexpr const char* APP_FILES_DIRECTORY = "/res/apps";
    static NonnullRefPtr<AppFile> get_for_app(const StringView& app_name);
    static NonnullRefPtr<AppFile> open(const StringView& path);
    static void for_each(Function<void(NonnullRefPtr<AppFile>)>, const StringView& directory = APP_FILES_DIRECTORY);
    ~AppFile();

    bool is_valid() const { return m_valid; }
    String filename() const { return m_config->filename(); }

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
    explicit AppFile(const StringView& path);

    bool validate() const;

    RefPtr<Core::ConfigFile> m_config;
    bool m_valid { false };
};

}
