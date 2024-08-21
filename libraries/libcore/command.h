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

#include <mods/lexical_path.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <spawn.h>

namespace Core
{
    /**
     * @param program 
     * @param arguments 
     * @param chdir 
     * @return String 
     */
    String command(const String& program, const Vector<String>& arguments, Optional<LexicalPath> chdir);

    /**
     * @param command_string 
     * @param chdir 
     * @return String 
     */
    String command(const String& command_string, Optional<LexicalPath> chdir);
} // namespace Core