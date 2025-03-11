/**
 * @file gzip.h
 * @author Krisna Pranav
 * @brief gzip
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcompress/deflate.h>
#include <libcrypto/checksum/crc32.h>

namespace Compress 
{

    constexpr u8 gzip_magic_1 = 0x1f;
    constexpr u8 gzip_magic_2 = 0x8b;

    struct [[gnu::packed]] BlockHeader {
        u8 identification_1;
        u8 identification_2;
        u8 compression_method;
        u8 flags;
        LittleEndian<u32> modification_time;
        u8 extra_flags;
        u8 operating_system;

        bool valid_magic_number() const;
        bool supported_by_implementation() const;
    }; // struct [[gnu::packed]] BlockHeader

    struct Flags {
        static constexpr u8 FTEXT = 1 << 0;
        static constexpr u8 FHCRC = 1 << 1;
        static constexpr u8 FEXTRA = 1 << 2;
        static constexpr u8 FNAME = 1 << 3;
        static constexpr u8 FCOMMENT = 1 << 4;

        static constexpr u8 MAX = FTEXT | FHCRC | FEXTRA | FNAME | FCOMMENT;
    }; // struct Flags

    class GzipDecompressor final : public InputStream 
    {
    public:
        /**
         * @brief Construct a new GzipDecompressor object
         * 
         */
        GzipDecompressor(InputStream&);

        /**
         * @brief Destroy the GzipDecompressor object
         * 
         */
        ~GzipDecompressor();

        /**
         * @return size_t 
         */
        size_t read(Bytes) override;

        /**
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes) override;
        bool discard_or_error(size_t) override;

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override;
        bool handle_any_error() override;

        /**
         * @return Optional<ByteBuffer> 
         */
        static Optional<ByteBuffer> decompress_all(ReadonlyBytes);

        /**
         * @return Optional<String> 
         */
        static Optional<String> describe_header(ReadonlyBytes);

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        static bool is_likely_compressed(ReadonlyBytes bytes);

    private:
        class Member 
        {
        public:
            /**
             * @brief Construct a new Member object
             * 
             * @param header 
             * @param stream 
             */
            Member(BlockHeader header, InputStream& stream)
                : m_header(header)
                , m_stream(stream)
            {
            }

            BlockHeader m_header;
            DeflateDecompressor m_stream;
            Crypto::Checksum::CRC32 m_checksum;
            size_t m_nread { 0 };
        }; // class Member

        /**
         * @return Member const& 
         */
        Member const& current_member() const 
        { 
            return m_current_member.value(); 
        }

        /**
         * @return Member& 
         */
        Member& current_member() 
        { 
            return m_current_member.value(); 
        }

        InputStream& m_input_stream;
        u8 m_partial_header[sizeof(BlockHeader)];
        size_t m_partial_header_offset { 0 };
        Optional<Member> m_current_member;

        bool m_eof { false };
    }; // class GzipDecompress final : public InputStream

    class GzipCompressor final : public OutputStream 
    {
    public:
        /**
         * @brief Construct a new GzipCompressor object
         * 
         */
        GzipCompressor(OutputStream&);

        /**
         * @brief Destroy the GzipCompressor object
         * 
         */
        ~GzipCompressor() = default;

        /**
         * @return size_t 
         */
        size_t write(ReadonlyBytes) override;

        /**
         * @return true 
         * @return false 
         */
        bool write_or_error(ReadonlyBytes) override;

        /**
         * @param bytes 
         * @return Optional<ByteBuffer> 
         */
        static Optional<ByteBuffer> compress_all(ReadonlyBytes bytes);

    private:
        OutputStream& m_output_stream;
    }; // class GzipCompressor final : public OutputStream 

} // namespace Compress
