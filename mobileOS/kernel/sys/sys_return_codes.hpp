/**
 * @file sys_return_codes.hpp
 * @author Krisna Pranav
 * @brief System Return Codes
 * @version 6.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace sys
{
enum class ReturnCodes
{
    Success,
    Failure,
    Timeout,
    ServicesDoesntExists,
    Unresolved
};
}