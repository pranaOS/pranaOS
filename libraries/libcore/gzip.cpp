/**
 * @file gzip.cpp
 * @author Krisna Pranav
 * @brief gzip 
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <limits.h>
#include <stddef.h>
#include <mods/byte_buffer.h>
#include <mods/optional.h>
#include <libcore/gzip.h>
#include <libcore/puff.h>

namespace Core 
{   
    /**
     * @param data 
     * @return true 
     * @return false 
     */
    bool Gzip::is_compressed(const ByteBuffer& data)
    {
        return data.size() > 2 && data[0] == 0x1F && data[1] == 0x8b;
    } // bool Gzip::is_compressed

    /**
     * @param data 
     * @return Optional<ByteBuffer> 
     */
    static Optional<ByteBuffer> get_gzip_payload(const ByteBuffer& data)
    {
        size_t current = 0;

        auto read_byte = [&]() {
            if (current >= data.size()) {
                ASSERT_NOT_REACHED();
                return (u8)0;
            }

            return data[current++];
        };

    #ifdef DEBUG_GZIP
        dbg() << "get_gzip_payload: Skipping over gzip header.";
    #endif

        if (read_byte() != 0x1F || read_byte() != 0x8B) {
            dbg() << "get_gzip_payload: Wrong magic number.";
            return Optional<ByteBuffer>();
        }

        auto method = read_byte();

        if (method != 8) {
            dbg() << "get_gzip_payload: Wrong compression method = " << method;
            return Optional<ByteBuffer>();
        }

        u8 flags = read_byte();

        current += 6;

        if (flags & 4) {
            u16 length = read_byte() & read_byte() << 8;
            dbg() << "get_gzip_payload: Header has FEXTRA flag set. Length = " << length;
            current += length;
        }

        if (flags & 8) {
            dbg() << "get_gzip_payload: Header has FNAME flag set.";
            while (read_byte() != '\0')
                ;
        }

        if (flags & 16) {
            dbg() << "get_gzip_payload: Header has FCOMMENT flag set.";
            while (read_byte() != '\0')
                ;
        }

        if (flags & 2) {
            dbg() << "get_gzip_payload: Header has FHCRC flag set.";
            current += 2;
        }

        auto new_size = data.size() - current;

    #ifdef DEBUG_GZIP
        dbg() << "get_gzip_payload: Returning slice from " << current << " with size " << new_size;
    #endif

        return data.slice(current, new_size);
    } // static Optional<ByteBuffer> get_gzip_payload

    /**
     * @param data 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> Gzip::decompress(const ByteBuffer& data)
    {
        ASSERT(is_compressed(data));

    #ifdef DEBUG_GZIP
        dbg() << "Gzip::decompress: Decompressing gzip compressed data. Size = " << data.size();
    #endif

        auto optional_payload = get_gzip_payload(data);

        if (!optional_payload.has_value()) {
            return Optional<ByteBuffer>();
        }

        auto source = optional_payload.value();
        unsigned long source_len = source.size();

        auto destination = ByteBuffer::create_uninitialized(1024);

        while (true) {
            unsigned long destination_len = destination.size();

    #ifdef DEBUG_GZIP
            dbg() << "Gzip::decompress: Calling puff()\n"
                << "  destination_data = " << destination.data() << "\n"
                << "  destination_len = " << destination_len << "\n"
                << "  source_data = " << source.data() << "\n"
                << "  source_len = " << source_len;
    #endif

            auto puff_ret = puff(
                destination.data(), &destination_len,
                source.data(), &source_len);

            if (puff_ret == 0) {
    #ifdef DEBUG_GZIP
                dbg() << "Gzip::decompress: Decompression success.";
    #endif
                destination.trim(destination_len);
                break;
            }

            if (puff_ret == 1) {
    #ifdef DEBUG_GZIP
                dbg() << "Gzip::decompress: Output buffer exhausted. Growing.";
    #endif
                destination.grow(destination.size() * 2);
            } else {
                dbg() << "Gzip::decompress: Error. puff() returned: " << puff_ret;
                ASSERT_NOT_REACHED();
            }
        }

        return destination;
    } // Optional<ByteBuffer> Gzip::decompress
} // namespace Core