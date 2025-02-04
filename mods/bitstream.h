/**
 * @file bitstream.h
 * @author Krisna Pranav
 * @brief bit stream
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/stream.h>

namespace Mods
{
    class InputBitStream final : public InputStream 
    {   
    public:
        /**
         * @brief Construct a new Input Bit Stream object
         * 
         * @param stream 
         */
        explicit InputBitStream(InputStream& stream)
            : m_stream(stream)
        {}

        
    private:
        Optional<u8> m_next_byte;
        size_t m_bit_offset { 0 };
        InputStream& m_stream;
    }; // class InputBitStream final : public InputStream
} // namespace Mods