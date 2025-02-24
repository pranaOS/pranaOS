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

    struct Flags {};

    class GzipDecompressor final : public InputStream
    {

    }; // class GzipDecompressor final : public InputStream

    
} // namespace Compress