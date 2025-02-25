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