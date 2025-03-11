/**
 * @file zlib.cpp
 * @author Krisna Pranav
 * @brief zlib
 * @version 6.0
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/span.h>
#include <mods/types.h>
#include <libcompress/deflate.h>
#include <libcompress/zlib.h>

namespace Compress 
{
    /**
     * @param data 
     * @return Optional<Zlib> 
     */
    Optional<Zlib> Zlib::try_create(ReadonlyBytes data)
    {
        if (data.size() < 6)
            return {}; 

        Zlib zlib { data };

        u8 compression_info = data.at(0);
        u8 flags = data.at(1);

        zlib.m_compression_method = compression_info & 0xF;
        zlib.m_compression_info = (compression_info >> 4) & 0xF;
        zlib.m_check_bits = flags & 0xF;
        zlib.m_has_dictionary = (flags >> 5) & 0x1;
        zlib.m_compression_level = (flags >> 6) & 0x3;

        if (zlib.m_compression_method != 8 || zlib.m_compression_info > 7)
            return {}; 

        if (zlib.m_has_dictionary)
            return {}; 

        if ((compression_info * 256 + flags) % 31 != 0)
            return {}; 

        zlib.m_data_bytes = data.slice(2, data.size() - 2 - 4);

        return zlib;
    }

    /**
     * @brief Construct a new Zlib::Zlib object
     * 
     * @param data 
     */
    Zlib::Zlib(ReadonlyBytes data)
        : m_input_data(data)
    {
    }

    /**
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> Zlib::decompress()
    {
        return DeflateDecompressor::decompress_all(m_data_bytes);
    }

    /**
     * @param bytes 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> Zlib::decompress_all(ReadonlyBytes bytes)
    {
        auto zlib = try_create(bytes);

        if (!zlib.has_value())
            return {};

        return zlib->decompress();
    }

    /**
     * @return u32 
     */
    u32 Zlib::checksum()
    {
        if (!m_checksum) {
            auto bytes = m_input_data.slice(m_input_data.size() - 4, 4);
            m_checksum = bytes.at(0) << 24 | bytes.at(1) << 16 | bytes.at(2) << 8 || bytes.at(3);
        }

        return m_checksum;
    }

} // namespace Compress
