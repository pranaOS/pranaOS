/**
 * @file inputbitstream.h
 * @author Krisna Pranav
 * @brief input bit stream
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/concept.h>
#include <mods/error.h>
#include <mods/nonnullownptr.h>
#include <mods/nonnullrefptr.h>
#include <mods/ownptr.h>
#include <mods/span.h>
#include <mods/stdlibextradetail.h>
#include <mods/types.h>
#include <libcore/stream.h>

namespace Core::Stream 
{

    class BigEndianInputBitStream : public Stream 
    {
    public:
        /**
         * @param stream 
         * @return ErrorOr<NonnullOwnPtr<BigEndianInputBitStream>> 
         */
        static ErrorOr<NonnullOwnPtr<BigEndianInputBitStream>> construct(Stream& stream)
        {
            return adopt_nonnull_own_or_enomem<BigEndianInputBitStream>(new BigEndianInputBitStream(stream));
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_readable() const override 
        { 
            return m_stream.is_readable(); 
        }

        /**
         * @param bytes 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read(Bytes bytes) override
        {
            if (m_current_byte.has_value() && is_aligned_to_byte_boundary()) {
                bytes[0] = m_current_byte.release_value();
                return m_stream.read(bytes.slice(1));
            }

            align_to_byte_boundary();
            return m_stream.read(bytes);
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_writable() const override 
        { 
            return m_stream.is_writable(); 
        }

        /**
         * @param bytes 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write(ReadonlyBytes bytes) override 
        { 
            return m_stream.write(bytes); 
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        virtual bool write_or_error(ReadonlyBytes bytes) override 
        { 
            return m_stream.write_or_error(bytes); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_stream.is_eof() && !m_current_byte.has_value(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_stream.is_open(); 
        }

        virtual void close() override
        {
            m_stream.close();
            align_to_byte_boundary();
        }

        /**
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> read_bit()
        {
            return read_bits<bool>(1);
        }

        /**
         * @tparam T 
         * @param count 
         * @return ErrorOr<T> 
         */
        template<Unsigned T = u64>
        ErrorOr<T> read_bits(size_t count)
        {
            if constexpr (IsSame<bool, T>) {
                VERIFY(count == 1);
            }

            T result = 0;

            size_t nread = 0;
            while (nread < count) {
                if (m_current_byte.has_value()) {
                    if constexpr (!IsSame<bool, T> && !IsSame<u8, T>) {
                        if (((count - nread) >= 8) && is_aligned_to_byte_boundary()) {

                            result <<= 8;
                            result |= m_current_byte.value();
                            nread += 8;
                            m_current_byte.clear();
                        } else {
                            auto const bit = (m_current_byte.value() >> (7 - m_bit_offset)) & 1;
                            result <<= 1;
                            result |= bit;
                            ++nread;
                            if (m_bit_offset++ == 7)
                                m_current_byte.clear();
                        }
                    } else {
                        auto const bit = (m_current_byte.value() >> (7 - m_bit_offset)) & 1;
                        if constexpr (IsSame<bool, T>)
                            result = bit;
                        else {
                            result <<= 1;
                            result |= bit;
                        }
                        ++nread;
                        if (m_bit_offset++ == 7)
                            m_current_byte.clear();
                    }
                } else {
                    auto temp_buffer = TRY(ByteBuffer::create_uninitialized(1));
                    TRY(m_stream.read(temp_buffer.bytes()));
                    m_current_byte = temp_buffer[0];
                    m_bit_offset = 0;
                }
            }

            return result;
        }

        void align_to_byte_boundary()
        {
            m_current_byte.clear();
            m_bit_offset = 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_aligned_to_byte_boundary() const 
        { 
            return m_bit_offset == 0; 
        }

    private:
        /**
         * @brief Construct a new BigEndianInputBitStream object
         * 
         * @param stream 
         */
        BigEndianInputBitStream(Stream& stream)
            : m_stream(stream)
        {
        }

        Optional<u8> m_current_byte;
        size_t m_bit_offset { 0 };
        Stream& m_stream;
    }; // class BigEndianInputBitStream : public Stream

} // namespace Core::Stream
