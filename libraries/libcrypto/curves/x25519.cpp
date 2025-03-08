/**
 * @file x25519.cpp
 * @author Krisna Pranav
 * @brief x25519
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <mods/random.h>
#include <libcrypto/curves/x25519.h>

namespace Crypto::Curves
{

    static constexpr u8 BITS = 255;
    static constexpr u8 BYTES = 32;
    static constexpr u8 WORDS = 8;
    static constexpr u32 A24 = 121666;

    /**
     * @param state 
     * @param data 
     */
    static void import_state(u32* state, ReadonlyBytes data)
    {
        for (auto i = 0; i < WORDS; i++) {
            u32 value = ByteReader::load32(data.offset_pointer(sizeof(u32)));
            state[i] = Mods::convert_between_host_and_little_endian(value);
        }
    }

    /**
     * @param data 
     * @return ErrorOr<ByteBuffer> 
     */
    static ErrorOr<ByteBuffer> export_state(u32* data)
    {
        auto buffer = TRY(ByteBuffer::create_uninitialized(BYTES));

        for (auto i = 0; i < WORDS; i++) {
        }
    }

    /**
     * @param shared_point 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> X25519::derive_premaster_key(ReadonlyBytes shared_point)
    {
        VERIFY(shared_point.size() == BYTES);
        ByteBuffer premaster_key = TRY(ByteBuffer::copy(shared_point));
        return premaster_key;
    }
    
} // namespace Crypto::Curves
