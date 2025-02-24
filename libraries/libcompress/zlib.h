/**
 * @file zlib.h
 * @author Krisna Pranav
 * @brief zlib
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/optional.h>

namespace Compress
{
    class Zlib
    {
    private:
        Zlib(ReadonlyBytes data);
    }; // class Zlib
} // namespace Compress