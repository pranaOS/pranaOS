/**
 * @file gzip.h
 * @author Krisna Pranav
 * @brief gzip
 * @version 6.0
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/optional.h>
#include <mods/string.h>

namespace Core 
{
    class Gzip 
    {
    public:
        /**
         * @param data 
         * @return true 
         * @return false 
         */
        static bool is_compressed(const ByteBuffer& data);

        /**
         * @param data 
         * @return Optional<ByteBuffer> 
         */
        static Optional<ByteBuffer> decompress(const ByteBuffer& data);
    }; // class Gzip
} // namespace Core