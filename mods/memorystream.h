/**
 * @file memorystream.h
 * @author Krisna Pranav
 * @brief Memory Stream
 * @version 6.0
 * @date 2024-11-10
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/leb128.h>
#include <mods/memmem.h>
#include <mods/stream.h>
#include <mods/vector.h>

namespace Mods
{
    class InputMemoryStream final : public InputStream
    {
    public:
        /**
         * @brief Construct a new Input Memory Stream object
         *
         * @param bytes
         */
        explicit InputMemoryStream(ReadonlyBytes bytes)
            : m_bytes(bytes)
        {
        }

        bool unreliable_eof() const override
        {
            return eof();
        }

        bool eof() const
        {
            return m_offset >= m_bytes.size();
        }

        /**
         * @param bytes
         * @return size_t
         */
        size_t read(Bytes bytes) override
        {
            if(has_any_error())
                return 0;

            auto const count = min(bytes.size(), remaining());
            __builtin_memcpy(bytes.data(), m_bytes.data() + m_offset, count);
            m_offset += count;
            return count;
        }

        /**
         * @param bytes
         * @return true
         * @return false
         */
        bool read_or_error(Bytes bytes) override
        {
            if(remaining() < bytes.size())
            {
                set_recoverable_error();
                return false;
            }

            __builtin_memcpy(bytes.data(), m_bytes.data() + m_offset, bytes.size());
            m_offset += bytes.size();
            return true;
        }

        /**
         * @param count
         * @return true
         * @return false
         */
        bool discard_or_error(size_t count) override
        {
            if(remaining() < count)
            {
                set_recoverable_error();
                return false;
            }

            m_offset += count;
            return true;
        }

        /**
         * @param offset
         */
        void seek(size_t offset)
        {
            VERIFY(offset < m_bytes.size());
            m_offset = offset;
        }

        /**
         * @return u8
         */
        u8 peek_or_error() const
        {
            if(remaining() == 0)
            {
                set_recoverable_error();
                return 0;
            }

            return m_bytes[m_offset];
        }

        /**
         * @tparam ValueType
         * @param result
         * @return true
         * @return false
         */
        template <typename ValueType>
        bool read_LEB128_unsigned(ValueType& result)
        {
            return LEB128::read_unsigned(*this, result);
        }

        /**
         * @tparam ValueType
         * @param result
         * @return true
         * @return false
         */
        template <typename ValueType>
        bool read_LEB128_signed(ValueType& result)
        {
            return LEB128::read_signed(*this, result);
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

        /**
         * @return size_t
         */
        size_t remaining() const
        {
            return m_bytes.size() - m_offset;
        }

    private:
        ReadonlyBytes m_bytes;
        size_t m_offset{0};
    }; // class InputMemoryStream final : public InputStream

    class OutputMemoryStream final : public OutputStream
    {
    public:
        /**
         * @brief Construct a new Output Memory Stream object
         *
         * @param bytes
         */
        explicit OutputMemoryStream(Bytes bytes)
            : m_bytes(bytes)
        {
        }

        /**
         * @param bytes
         * @return size_t
         */
        size_t write(ReadonlyBytes bytes) override
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
        bool write_or_error(ReadonlyBytes bytes) override
        {
            if(remaining() < bytes.size())
            {
                set_recoverable_error();
                return false;
            }

            write(bytes);
            return true;
        }

        /**
         * @param value
         * @return size_t
         */
        size_t fill_to_end(u8 value)
        {
            auto const nwritten = m_bytes.slice(m_offset).fill(value);
            m_offset += nwritten;
            return nwritten;
        }

        bool is_end() const
        {
            return remaining() == 0;
        }

        /**
         * @return ReadonlyBytes
         */
        ReadonlyBytes bytes() const
        {
            return {data(), size()};
        }

        /**
         * @return Bytes
         */
        Bytes bytes()
        {
            return {data(), size()};
        }

        /**
         * @return u8 const*
         */
        u8 const* data() const
        {
            return m_bytes.data();
        }

        /**
         * @return u8*
         */
        u8* data()
        {
            return m_bytes.data();
        }

        size_t size() const
        {
            return m_offset;
        }

        size_t remaining() const
        {
            return m_bytes.size() - m_offset;
        }

        void reset()
        {
            m_offset = 0;
        }

    private:
        size_t m_offset{0};
        Bytes m_bytes;
    }; // class OutputMemoryStream final : public OutputStream

    class DuplexMemoryStream final : public DuplexStream
    {
    public:
        static constexpr size_t chunk_size = 4 * 1024;

        /**
         * @return true
         * @return false
         */
        bool unreliable_eof() const override
        {
            return eof();
        }

        bool eof() const
        {
            return m_write_offset == m_read_offset;
        }

        /**
         * @param count
         * @return true
         * @return false
         */
        bool discard_or_error(size_t count) override
        {
            if(m_write_offset - m_read_offset < count)
            {
                set_recoverable_error();
                return false;
            }

            m_read_offset += count;
            try_discard_chunks();
            return true;
        }

        /**
         * @param value
         * @return Optional<size_t>
         */
        Optional<size_t> offset_of(ReadonlyBytes value) const
        {
            Vector<ReadonlyBytes> spans;
            auto chunk_index = (m_read_offset - m_base_offset) / chunk_size;
            auto chunk_read_offset = (m_read_offset - m_base_offset) % chunk_size;
            auto bytes_to_search = m_write_offset - m_read_offset;

            for(; bytes_to_search > 0;)
            {
                ReadonlyBytes span = m_chunks[chunk_index];
                if(chunk_read_offset)
                {
                    span = span.slice(chunk_read_offset);
                    chunk_read_offset = 0;
                }
                if(bytes_to_search < span.size())
                {
                    spans.append(span.slice(0, bytes_to_search));
                    break;
                }
                bytes_to_search -= span.size();
                spans.append(move(span));
                ++chunk_index;
            }

            return memmem(spans.begin(), spans.end(), value);
        }

        /**
         * @param bytes
         * @return size_t
         */
        size_t read_without_consuming(Bytes bytes) const
        {
            size_t nread = 0;
            while(bytes.size() - nread > 0 && m_write_offset - m_read_offset - nread > 0)
            {
                auto const chunk_index = (m_read_offset - m_base_offset + nread) / chunk_size;
                auto const chunk_bytes = m_chunks[chunk_index].bytes().slice((m_read_offset + nread) % chunk_size).trim(m_write_offset - m_read_offset - nread);
                nread += chunk_bytes.copy_trimmed_to(bytes.slice(nread));
            }

            return nread;
        }

        /**
         * @param bytes
         * @return size_t
         */
        size_t read(Bytes bytes) override
        {
            if(has_any_error())
                return 0;

            auto const nread = read_without_consuming(bytes);

            m_read_offset += nread;
            try_discard_chunks();

            return nread;
        }

        /**
         * @param bytes
         * @return true
         * @return false
         */
        bool read_or_error(Bytes bytes) override
        {
            if(m_write_offset - m_read_offset < bytes.size())
            {
                set_recoverable_error();
                return false;
            }

            return read(bytes) == bytes.size();
        }

        /**
         * @param bytes
         * @return size_t
         */
        size_t write(ReadonlyBytes bytes) override
        {
            size_t nwritten = 0;

            while(bytes.size() - nwritten > 0)
            {
                if((m_write_offset + nwritten) % chunk_size == 0)
                    m_chunks.append(ByteBuffer::create_uninitialized(chunk_size).release_value_but_fixme_should_propagate_errors()); // FIXME: Handle possible OOM situation.

                nwritten += bytes.slice(nwritten).copy_trimmed_to(m_chunks.last().bytes().slice((m_write_offset + nwritten) % chunk_size));
            }

            m_write_offset += nwritten;
            return nwritten;
        }

        /**
         * @param bytes
         * @return true
         * @return false
         */
        bool write_or_error(ReadonlyBytes bytes) override
        {
            write(bytes);
            return true;
        }

        /**
         * @return ByteBuffer
         */
        ByteBuffer copy_into_contiguous_buffer() const
        {
            auto buffer = ByteBuffer::create_uninitialized(size()).release_value_but_fixme_should_propagate_errors();

            auto const nread = read_without_consuming(buffer);
            VERIFY(nread == buffer.size());

            return buffer;
        }

        size_t roffset() const
        {
            return m_read_offset;
        }

        size_t woffset() const
        {
            return m_write_offset;
        }

        size_t size() const
        {
            return m_write_offset - m_read_offset;
        }

    private:
        void try_discard_chunks()
        {
            while(m_read_offset - m_base_offset >= chunk_size)
            {
                m_chunks.take_first();
                m_base_offset += chunk_size;
            }
        }

        Vector<ByteBuffer> m_chunks;
        size_t m_write_offset{0};
        size_t m_read_offset{0};
        size_t m_base_offset{0};
    }; // class DuplexMemoryStream final : public DuplexStream

} // namespace Mods

using Mods::DuplexMemoryStream;
using Mods::InputMemoryStream;
using Mods::InputStream;
using Mods::OutputMemoryStream;
