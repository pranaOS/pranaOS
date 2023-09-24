/**
 * @file getpassword.h
 * @author Krisna Pranav
 * @brief get password
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/result.h>
#include <mods/string.h>

namespace Core
{
    /**
     * @param prompt 
     * @return Result<String, int> 
     */
    Result<String, int> get_password(const StringView& prompt = "Password: ");
} // namespace Core