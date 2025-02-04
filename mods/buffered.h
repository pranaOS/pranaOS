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
    public:
        size_t buffered() const
        {
            return m_buffered;
        }

        StreamType& underlying_stream() 
        {
            return m_stream;
        }
    private:
        Bytes buffer() const
        {
            return { m_buffer, Size };
        }

        mutable StreamType m_stream;
        mutable u8 m_buffer[Size];
        mutable size_t m_buffered { 0 };
    }; // class Buffered<StreamType, Size> final : public InputStream 
} // namespace Mods