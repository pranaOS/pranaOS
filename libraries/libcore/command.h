/**
 * @file command.h
 * @author Krisna Pranav
 * @brief command
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/lexicalpath.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <spawn.h>

namespace Core 
{

    struct CommandResult {
        int exit_code { 0 };
        String output;
        String error;
    }; // struct CommandResult

    /**
     * @param program 
     * @param arguments 
     * @param chdir 
     * @return ErrorOr<CommandResult> 
     */
    ErrorOr<CommandResult> command(String const& program, Vector<String> const& arguments, Optional<LexicalPath> chdir);
    
    /**
     * @param command_string 
     * @param chdir 
     * @return ErrorOr<CommandResult> 
     */
    ErrorOr<CommandResult> command(String const& command_string, Optional<LexicalPath> chdir);

} // namespace Core
