/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/LexicalPath.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <libcore/StandardPaths.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

namespace Core {

String StandardPaths::home_directory()
{
    if (auto* home_env = getenv("HOME"))
        return LexicalPath::canonicalized_path(home_env);

    auto* pwd = getpwuid(getuid());
    String path = pwd ? pwd->pw_dir : "/";
    endpwent();
    return LexicalPath::canonicalized_path(path);
}

String StandardPaths::desktop_directory()
{
    StringBuilder builder;
    builder.append(home_directory());
    builder.append("/Desktop");
    return LexicalPath::canonicalized_path(builder.to_string());
}

String StandardPaths::downloads_directory()
{
    StringBuilder builder;
    builder.append(home_directory());
    builder.append("/Downloads");
    return LexicalPath::canonicalized_path(builder.to_string());
}

String StandardPaths::config_directory()
{
    StringBuilder builder;
    builder.append(home_directory());
    builder.append("/.config");
    return LexicalPath::canonicalized_path(builder.to_string());
}

String StandardPaths::tempfile_directory()
{
    return "/tmp";
}

}
