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
} // namespace Core