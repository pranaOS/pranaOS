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
#include <libcore/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

namespace Core {

#ifdef __pranaos__

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

    for (auto const& part : arguments) {
        parts.append(part.characters());
    }

    parts.append(nullptr);

    char const** argv = parts.data();

    posix_spawn_file_actions_t action;
    
    posix_spawn_file_actions_init(&action);

    if (chdir.has_value()) {
        posix_spawn_file_actions_addchdir(&action, chdir.value().string().characters());
    }

    posix_spawn_file_actions_adddup2(&action, stdout_pipe[1], STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&action, stderr_pipe[1], STDERR_FILENO);

    pid_t pid;

    if ((errno = posix_spawnp(&pid, program.characters(), &action, nullptr, const_cast<char**>(argv), environ))) {
        perror("posix_spawn");
        VERIFY_NOT_REACHED();
    }

    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    auto read_all_from_pipe = [](int pipe[2]) {
        auto result_file = Core::File::construct();
        if (!result_file->open(pipe[0], Core::OpenMode::ReadOnly, Core::File::ShouldCloseFileDescriptor::Yes)) {
            perror("open");
            VERIFY_NOT_REACHED();
        }
        return String::copy(result_file->read_all());
    };
    
    auto output = read_all_from_pipe(stdout_pipe);
    auto error = read_all_from_pipe(stderr_pipe);

    int wstatus { 0 };
    waitpid(pid, &wstatus, 0);
    posix_spawn_file_actions_destroy(&action);
    int exit_code = WEXITSTATUS(wstatus);

    if (exit_code != 0) {
#    ifdef DBG_FAILED_COMMANDS
        dbgln("command failed. stderr: {}", );
#    endif
    }

    return CommandResult { WEXITSTATUS(wstatus), output, error };
}

#endif

}
