/**
 * @file zlib.h
 * @author Krisna Pranav
 * @brief zlib
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/types.h>

namespace Compress 
{

    class Zlib 
    {
    public:
        /**
         * @return Optional<ByteBuffer> 
         */
        Optional<ByteBuffer> decompress();

        /**
         * @return u32 
         */
        u32 checksum();

        /**
         * @param data 
         * @return Optional<Zlib> 
         */
        static Optional<Zlib> try_create(ReadonlyBytes data);

        /**
         * @return Optional<ByteBuffer> 
         */
        static Optional<ByteBuffer> decompress_all(ReadonlyBytes);

    private:
        /**
         * @brief Construct a new Zlib object
         * 
         * @param data 
         */
        Zlib(ReadonlyBytes data);

        u8 m_compression_method;
        u8 m_compression_info;
        u8 m_check_bits;
        u8 m_has_dictionary;
        u8 m_compression_level;

        u32 m_checksum { 0 };
        ReadonlyBytes m_input_data;
        ReadonlyBytes m_data_bytes;
    }; // class Zlib

} // namespace Compress 
