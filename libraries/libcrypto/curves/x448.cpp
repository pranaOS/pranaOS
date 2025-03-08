/**
 * @file x448.cpp
 * @author Krisna Pranav
 * @brief x448
 * @version 6.0
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/endian.h>
#include <mods/random.h>
#include <libcrypto/curves/x448.h>

namespace Crypto::Curves
{

    static constexpr u16 BITS = 448;
    static constexpr u8 BYTES = 56;
    static constexpr u8 WORDS = 14;
    static constexpr u32 A24 = 39092;

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
     * @param state 
     * @param value 
     */
    static void set(u32* state, u32 value)
    {
        state[0] = value;

        for (auto i = 1; i < WORDS; i++) {
            state[i] = 0;
        }
    }

    /**
     * @param state 
     * @param value 
     */
    static void copy(u32* state, u32* value)
    {
        for (auto i = 0; i < WORDS; i++) {
            state[i] = value[i];
        }
    }

    /**
     * @param first 
     * @param second 
     * @param condition 
     */
    static void conditional_swap(u32* first, u32* second, u32 condition)
    {
        u32 mask = ~condition + 1;

        for (auto i = 0; i < WORDS; i++) {
            u32 temp = mask & (first[i] ^ second[i]);
            first[i] ^= temp;
            second[i] ^= temp;
        }
    }

    /**
     * @param state 
     * @param data 
     * @param a_high 
     */
    static void modular_reduce(u32* state, u32* data, u32 a_high)
    {
        u64 temp = 1;
        u32 other[WORDS];

        for (auto i = 0; i < WORDS / 2; i++) {
            temp += data[i];
            other[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        temp += 1;

        for (auto i = 7; i < WORDS; i++) {
            temp += data[i];
            other[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        auto condition = (a_high + (u32)temp - 1) & 1;
        select(state, other, data, condition);
    }

    /**
     * @param a 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> X448::generate_public_key(ReadonlyBytes a)
    {
        u8 generator[BYTES] { 5 };
        return compute_coordinate(a, { generator, BYTES });
    }
    
} // namespace Crypto::Curves