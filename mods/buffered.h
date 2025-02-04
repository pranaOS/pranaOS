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
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto nread = buffer().trim(m_buffered).copy_trimmed_to(bytes);

            m_buffered -= nread;

            if (m_buffered > 0)
                buffer().slice(nread, m_buffered).copy_to(buffer());

            if (nread < bytes.size()) {
                nread += m_stream.read(bytes.slice(nread));

                m_buffered = m_stream.read(buffer());
            }

            return nread;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (read(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

        bool unreliable_eof() const override 
        { 
            return m_buffered == 0 && m_stream.unreliable_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool eof() const
        {
            if (m_buffered > 0)
                return false;
            
            m_buffered = m_stream.read(buffer());

            return m_buffered = 0;
        }

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override
        {
            size_t ndiscarded = 0;

            while (ndiscarded < count) {
                u8 dummy[Size];

                if (!read_or_error({ dummy, min(Size, count - ndiscarded) }))
                    return false;
                
                ndiscarded += min(Size, count - ndiscarded);
            }

            return true;
        }

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