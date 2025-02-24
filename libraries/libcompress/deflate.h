/**
 * @file deflate.h
 * @author Krisna Pranav
 * @brief deflate
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bitstream.h>
#include <mods/bytebuffer.h>
#include <mods/circularduplexstream.h>
#include <mods/endian.h>
#include <mods/vector.h>
#include <libcompress/deflatetables.h>

namespace Compress
{
    class CanonicalCode 
    {
    public:
        /**
         * @brief Construct a new Canonical Code object
         * 
         */
        CanonicalCode() = default;

        /**
         * @return u32 
         */
        u32 read_symbol(InputBitStream&) const;

        void write_symbol(OutputBitStream&, u32) const;

        static CanonicalCode const& fixed_literal_codes();
        static CanonicalCode const& fixed_distance_codes();

        static Optional<CanonicalCode> from_bytes(ReadonlyBytes);

    private:
        Vector<u16> m_symbol_codes;
        Vector<u16> m_symbol_values;

        Array<u16, 288> m_bit_codes {}; 
        Array<u16, 288> m_bit_code_lengths {};
    }; // class CanonicalCode 
} // namespace Compress