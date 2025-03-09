/**
 * @file process.cpp
 * @author Krisna Pranav
 * @brief process
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string.h>
#include <libcore/process.h>
#include <errno.h>
#include <spawn.h>

#ifdef __pranaos__
#    include <pranaos.h>
#endif

extern char** environ;

namespace Core 
{

    /**
     * @param path 
     * @return pid_t 
     */
    pid_t Process::spawn(StringView path)
    {
        String path_string = path;

        pid_t pid;
        char const* argv[] = { path_string.characters(), nullptr };
        if ((errno = posix_spawn(&pid, path_string.characters(), nullptr, nullptr, const_cast<char**>(argv), environ))) {
            perror("Process::spawn posix_spawn");
        } else {
    #ifdef __serenity__
            if (disown(pid) < 0)
                perror("Process::spawn disown");
    #endif
        }
        return pid;
    }

} // namespace Core
