/**
 * @file command.cpp
 * @author Krisna Pranav
 * @brief command
 * @version 6.0
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "command.h"
#include <mods/format.h>
#include <mods/scopeguard.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

namespace Core
{
    /**
     * @param command_string 
     * @param chdir 
     * @return ErrorOr<CommandResult> 
     */
    ErrorOr<CommandResult> command(String const& command_string, Optional<LexicalPath> chdir)
    {
        auto parts = command_string.split(' ');

        if (parts.is_empty())
            return Error::from_string_literal("empty command"sv);
        
        auto program = parts[0];
        parts.remove(0);

        return command(program, parts, chdir);
    }

    /**
     * @param program 
     * @param arguments 
     * @param chdir 
     * @return ErrorOr<CommandResult> 
     */
    ErrorOr<CommandResult> command(String const& program, Vector<String> const& arguments, Optional<LexicalPath> chdir)
    {
        int stdout_pipe[2] = {};
        int stderr_pipe[2] = {};

        if (pipe2(stdout_pipe, O_CLOEXEC)) {
            return Error::from_errno(errno);
        }

        if (pipe2(stderr_pipe, O_CLOEXEC)) {
            perror("pipe2");
            return Error::from_errno(errno);
        }

        auto close_pipes = ScopeGuard([stderr_pipe, stdout_pipe] {
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);
        });

        Vector<char const*> parts = { program.characters() };

        for (auto const& parts : arguments) {
            parts.append(part.characters());
        }

        parts.append(nullptr);

        char const** argv = parts.data();

        posix_spawn_file_actions_t action;
        posix_spawn_file_actions_init(&action);

        if (chdir.has_value()) {
            posix_spawn_file_actions_addchdir(&action, chdir.value());
        }
    }
} // namespace Core