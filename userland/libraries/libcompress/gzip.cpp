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
     * @return true 
     * @return false 
     */
    bool BlockHeader::supported_by_implementation() const
    {
        if (compression_method != 0x08) {
            return false;
        }

        if (flags > Flags::MAX) {
            return false;
        }

        return true;
    }

    /**
     * @brief Construct a new GzipDecompressor::GzipDecompressor object
     * 
     * @param stream 
     */
    GzipDecompressor::GzipDecompressor(InputStream& stream)
        : m_input_stream(stream)
    {
    }

    /**
     * @brief Destroy the GzipDecompressor::GzipDecompressor object
     * 
     */
    GzipDecompressor::~GzipDecompressor()
    {
        m_current_member.clear();
    }

    /**
     * @param bytes 
     * @return size_t 
     */
    size_t GzipDecompressor::read(Bytes bytes)
    {
        size_t total_read = 0;

        while (total_read < bytes.size()) {
            if (has_any_error() || m_eof)
                break;

            auto slice = bytes.slice(total_read);

            if (m_current_member.has_value()) {
                size_t nread = current_member().m_stream.read(slice);
                current_member().m_checksum.update(slice.trim(nread));
                current_member().m_nread += nread;

                if (current_member().m_stream.handle_any_error()) {
                    set_fatal_error();
                    break;
                }

                if (nread < slice.size()) {
                    LittleEndian<u32> crc32, input_size;
                    m_input_stream >> crc32 >> input_size;

                    if (crc32 != current_member().m_checksum.digest()) {
                        set_fatal_error();
                        break;
                    }

                    if (input_size != current_member().m_nread) {
                        set_fatal_error();
                        break;
                    }

                    m_current_member.clear();

                    total_read += nread;
                    continue;
                }

                total_read += nread;
                continue;
            } else {
                m_partial_header_offset += m_input_stream.read(Bytes { m_partial_header, sizeof(BlockHeader) }.slice(m_partial_header_offset));

                if (m_input_stream.handle_any_error() || m_input_stream.unreliable_eof()) {
                    m_eof = true;
                    break;
                }

                if (m_partial_header_offset < sizeof(BlockHeader)) {
                    break; 
                }

                m_partial_header_offset = 0;

                BlockHeader header = *(reinterpret_cast<BlockHeader*>(m_partial_header));

                if (!header.valid_magic_number() || !header.supported_by_implementation()) {
                    set_fatal_error();
                    break;
                }

                if (header.flags & Flags::FEXTRA) {
                    LittleEndian<u16> subfield_id, length;
                    m_input_stream >> subfield_id >> length;
                    m_input_stream.discard_or_error(length);
                }

                auto discard_string = [&]() {
                    char next_char;
                    do {
                        m_input_stream >> next_char;
                        if (m_input_stream.has_any_error()) {
                            set_fatal_error();
                            break;
                        }
                    } while (next_char);
                };

                if (header.flags & Flags::FNAME) {
                    discard_string();
                    if (has_any_error())
                        break;
                }

                if (header.flags & Flags::FCOMMENT) {
                    discard_string();
                    if (has_any_error())
                        break;
                }

                if (header.flags & Flags::FHCRC) {
                    LittleEndian<u16> crc16;
                    m_input_stream >> crc16;
                }

                m_current_member.emplace(header, m_input_stream);
                continue;
            }
        }
        return total_read;
    }

    /**
     * @param bytes 
     * @return Optional<String> 
     */
    Optional<String> GzipDecompressor::describe_header(ReadonlyBytes bytes)
    {
        if (bytes.size() < sizeof(BlockHeader))
            return {};

        auto& header = *(reinterpret_cast<BlockHeader const*>(bytes.data()));

        if (!header.valid_magic_number() || !header.supported_by_implementation())
            return {};

        LittleEndian<u32> original_size = *reinterpret_cast<u32 const*>(bytes.offset(bytes.size() - sizeof(u32)));
        return String::formatted("last modified: {}, original size {}", Core::DateTime::from_timestamp(header.modification_time).to_string(), (u32)original_size);
    }

    /**
     * @param bytes 
     * @return true 
     * @return false 
     */
    bool GzipDecompressor::read_or_error(Bytes bytes)
    {
        if (read(bytes) < bytes.size()) {
            set_fatal_error();
            return false;
        }

        return true;
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
