/**
 * @file loader error.h
 * @author Krisna Pranav
 * @brief Loader Error
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/flystring.h>
#include <errno.h>

namespace Audio
{
    struct LoaderError
    {
        enum class Category : u32
        {
            Unknown,
            IO,
            Format,
            Internal,
            Unimplemented
        }; // enum class Category : u32
    }; // struct LoaderError
} // namespace Audio