/**
 * @file utilities.h
 * @author Krisna Pranav
 * @brief utilities
 * @version 0.1
 * @date 2025-08-19
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytestring.h>
#include <mods/error.h>
#include <mods/string.h>
#include <mods/vector.h>

void platform_init();

/**
 * @return ErrorOr<ByteString> 
 */
ErrorOr<ByteString> application_directory();

/**
 * @brief Get the paths for helper process object
 * 
 * @param process_name 
 * @return ErrorOr<Vector<ByteString>> 
 */
ErrorOr<Vector<ByteString>> get_paths_for_helper_process(StringView process_name);

extern ByteString s_serenity_resource_root;

/**
 * @return Optional<ByteString const&> 
 */
Optional<ByteString const&> mach_server_name();

/**
 * @brief Set the mach server name object
 * 
 * @param name 
 */
void set_mach_server_name(ByteString name);
