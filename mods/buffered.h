/**
 * @file buffered.h
 * @author Krisna Pranav
 * @brief buffered
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/stream.h>
#include <mods/types.h>
#include <mods/noncopyable.h>
#include <mods/kmalloc.h>

namespace Mods
{   
    template<typename StreamType, size_t Size = 4096>
    class Buffered;

    template<typename StreamType, size_t Size>
    requires(IsBaseOf<InputStream, StreamType>) class Buffered<StreamType, Size> final : public InputStream 
    {
        MOD_MAKE_NONCOPYABLE(Buffered);
    };
} // namespace Mods