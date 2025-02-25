/**
 * @file gzip.cpp
 * @author Krisna Pranav
 * @brief gzip
 * @version 6.0
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <libcompress/gzip.h>
#include <mods/memorystream.h>
#include <mods/string.h>
#include <libcore/datetime.h>

namespace Compress 
{
    /**
     * @param bytes 
     * @return true 
     * @return false 
     */
    bool GzipDecompressor::is_likely_compressed(ReadonlyBytes bytes)
    {
        return bytes.size() >= 2 && bytes[0] == gzip_magic_1 && bytes[1] == gzip_magic_2;
    }

    /**
     * @return true 
     * @return false 
     */
    bool BlockHeader::valid_magic_number() const
    {
        return identification_1 == gzip_magic_1 && identification_2 == gzip_magic_2;
    }

    /**
     * @param count 
     * @return true 
     * @return false 
     */
    bool GzipDecompressor::discard_or_error(size_t count)
    {
        u8 buffer[4096];
        size_t ndiscarded = 0;
        
        while (ndiscarded < count) {
            if (unreliable_eof()) {
                set_fatal_error();
                return false;
            }

            ndiscarded += read({ buffer, min<size_t>(count - ndiscarded, sizeof(buffer)) });
        }

        return true;
    }

    /**
     * @param bytes 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> GzipDecompressor::decompress_all(ReadonlyBytes bytes)
    {
        InputMemoryStream memory_stream { bytes };
        GzipDecompressor gzip_stream { memory_stream };
        DuplexMemoryStream output_stream;

        u8 buffer[4096];

        while (!gzip_stream.has_any_error() && !gzip_stream.unreliable_eof()) {
            auto const nread = gzip_stream.read({ buffer, sizeof(buffer) });
            output_stream.write_or_error({ buffer, nread });
        }

        if (gzip_stream.handle_any_error())
            return {};

        return output_stream.copy_into_contiguous_buffer();
    }

    /**
     * @return true 
     * @return false 
     */
    bool GzipDecompressor::unreliable_eof() const 
    { 
        return m_eof; 
    }

    /**
     * @return true 
     * @return false 
     */
    bool GzipDecompressor::handle_any_error()
    {
        bool handled_errors = m_input_stream.handle_any_error();
        return Stream::handle_any_error() || handled_errors;
    }

    /**
     * @brief Construct a new GzipCompressor::GzipCompressor object
     * 
     * @param stream 
     */
    GzipCompressor::GzipCompressor(OutputStream& stream)
        : m_output_stream(stream)
    {
    }

    /**
     * @param bytes 
     * @return size_t 
     */
    size_t GzipCompressor::write(ReadonlyBytes bytes)
    {
        BlockHeader header;
        header.identification_1 = 0x1f;
        header.identification_2 = 0x8b;
        header.compression_method = 0x08;
        header.flags = 0;
        header.modification_time = 0;
        header.extra_flags = 3;      
        header.operating_system = 3; 
        m_output_stream << Bytes { &header, sizeof(header) };
        DeflateCompressor compressed_stream { m_output_stream };
        VERIFY(compressed_stream.write_or_error(bytes));
        compressed_stream.final_flush();
        Crypto::Checksum::CRC32 crc32;
        crc32.update(bytes);
        LittleEndian<u32> digest = crc32.digest();
        LittleEndian<u32> size = bytes.size();
        m_output_stream << digest << size;
        
        return bytes.size();
    }

    /**
     * @param bytes 
     * @return true 
     * @return false 
     */
    bool GzipCompressor::write_or_error(ReadonlyBytes bytes)
    {
        if (write(bytes) < bytes.size()) {
            set_fatal_error();
            return false;
        }

        return true;
    }

    /**
     * @param bytes 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> GzipCompressor::compress_all(ReadonlyBytes bytes)
    {
        DuplexMemoryStream output_stream;
        GzipCompressor gzip_stream { output_stream };

        gzip_stream.write_or_error(bytes);

        if (gzip_stream.handle_any_error())
            return {};

        return output_stream.copy_into_contiguous_buffer();
    }

} // namespace Compress 