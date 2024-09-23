/**
 * @file lockmode.h
 * @author Krisna Pranav
 * @brief Lock Mode
 * @version 6.0
 * @date 2024-09-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Kernel 
{

    enum class LockMode : u8 
    {
        Unlocked,
        Shared,
        Exclusive
    }; // enum class LockMode : u8

} // namespace Kernel