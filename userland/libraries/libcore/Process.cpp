/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/String.h>
#include <libcore/Process.h>
#include <errno.h>
#include <spawn.h>

#ifdef __pranaos__
#    include <pranaos.h>
#endif

extern char** environ;

namespace Core {

pid_t Process::spawn(StringView path)
{
    String path_string = path;

    pid_t pid;
    char const* argv[] = { path_string.characters(), nullptr };
    if ((errno = posix_spawn(&pid, path_string.characters(), nullptr, nullptr, const_cast<char**>(argv), environ))) {
        perror("Process::spawn posix_spawn");
    } else {
#ifdef __pranaos__
        if (disown(pid) < 0)
            perror("Process::spawn disown");
#endif
    }
    return pid;
}

}
