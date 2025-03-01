/**
 * @file inputbitstream.h
 * @author Krisna Pranav
 * @brief input bit stream
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/concept.h>
#include <mods/error.h>

namespace Core::Stream
{

    class BigEndianInputBitStrean : public Stream
    {
    private:
        /**
         * @brief Construct a new BigEndianInputBitStrean object
         * 
         * @param stream 
         */
        BigEndianInputBitStrean(Stream& stream)
            : m_stream(stream)
        {}
        
        Optional<u8> m_current_byte;

        size_t m_bit_offset { 0 };
        Stream& m_stream;
    }; // class BigEndianInputBitStrean : public Stream
} // namespace Core::Stream
