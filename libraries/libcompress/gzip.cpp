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
        return bytes.size() >= 2 && bytes[0] == gzip_magic_1;
    }
} // namespace Compress 