/**
 * @file memorystream.h
 * @author Krisna Pranav
 * @brief memory stream
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/nonnullownptr.h>
#include <mods/ownptr.h>
#include <mods/span.h>
#include <mods/typedtransfer.h>
#include <libcore/stream.h>

namespace Core::Stream 
{

    class MemoryStream final : public SeekableStream 
    {
    public:
        /**
         * @param bytes 
         * @return ErrorOr<NonnullOwnPtr<MemoryStream>> 
         */
        static ErrorOr<NonnullOwnPtr<MemoryStream>> construct(Bytes bytes)
        {
            return adopt_nonnull_own_or_enomem<MemoryStream>(new (nothrow) MemoryStream(bytes));
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_offset >= m_bytes.size(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return true; 
        }
        
        virtual void close() override { }
        
        /**
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(off_t) override 
        { 
            return Error::from_errno(ENOTSUP); 
        }

        /**
         * @param bytes 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes bytes) override
        {
            auto to_read = min(remaining(), bytes.size());

            if (to_read == 0)
                return Bytes {};

            m_bytes.slice(m_offset, to_read).copy_to(bytes);
            m_offset += to_read;

            return bytes.trim(to_read);
        }

        /**
         * @param offset 
         * @param seek_mode 
         * @return ErrorOr<off_t> 
         */
        virtual ErrorOr<off_t> seek(i64 offset, SeekMode seek_mode = SeekMode::SetPosition) override
        {
            switch (seek_mode) {
            case SeekMode::SetPosition:
                if (offset >= static_cast<i64>(m_bytes.size()))
                    return Error::from_string_literal("Offset past the end of the stream memory"sv);

                m_offset = offset;
                break;
            case SeekMode::FromCurrentPosition:
                if (offset + static_cast<i64>(m_offset) >= static_cast<i64>(m_bytes.size()))
                    return Error::from_string_literal("Offset past the end of the stream memory"sv);

                m_offset += offset;
                break;
            case SeekMode::FromEndPosition:
                if (offset >= static_cast<i64>(m_bytes.size()))
                    return Error::from_string_literal("Offset past the start of the stream memory"sv);

                m_offset = m_bytes.size() - offset;
                break;
            }
            return static_cast<off_t>(m_offset);
        }

        /**
         * @param bytes 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes bytes) override
        {
            auto const nwritten = bytes.copy_trimmed_to(m_bytes.slice(m_offset));
            m_offset += nwritten;
            return nwritten;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        virtual bool write_or_error(ReadonlyBytes bytes) override
        {
            if (remaining() < bytes.size())
                return false;

            MUST(write(bytes));
            return true;
        }

        /**
         * @return Bytes 
         */
        Bytes bytes() 
        { 
            return m_bytes; 
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes bytes() const 
        { 
            return m_bytes; 
        }

        /**
         * @return size_t 
         */
        size_t offset() const 
        { 
            return m_offset; 
        }

        size_t remaining() const 
        { 
            return m_bytes.size() - m_offset; 
        }

    protected:
        /**
         * @brief Construct a new MemoryStream object
         * 
         * @param bytes 
         */
        explicit MemoryStream(Bytes bytes)
            : m_bytes(bytes)
        {
        }

    private:
        Bytes m_bytes;
        size_t m_offset { 0 };
    }; // class MemoryStream final : public SeekableStream

} // namespace Core::Stream
