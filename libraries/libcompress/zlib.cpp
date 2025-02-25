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
    }

    /**
     * @brief Construct a new Zlib::Zlib object
     * 
     * @param data 
     */
    Zlib::Zlib(ReadonlyBytes data)
        : m_input_data(data)
    {}

    /**
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> Zlib::decompress()
    {
        return DeflateCompressor::decompress_all(m_data_bytes);
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