/**
 * @file getpassword.h
 * @author Krisna Pranav
 * @brief get password
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <libcore/secretstring.h>

namespace Core 
{
    /**
     * @brief Get the password object
     * 
     * @param prompt 
     * @return ErrorOr<SecretString> 
     */
    ErrorOr<SecretString> get_password(StringView prompt = "Password: "sv);

} // namespace Core
