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
    static constexpr u32 A24 = 39082;

    /**
     * @param state 
     * @param data 
     */
    static void import_state(u32* state, ReadonlyBytes data)
    {
        for (auto i = 0; i < WORDS; i++) {
            u32 value = ByteReader::load32(data.offset_pointer(sizeof(u32) * i));
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
            u32 value = Mods::convert_between_host_and_little_endian(data[i]);
            ByteReader::store(buffer.offset_pointer(sizeof(u32) * i), value);
        }

        return buffer;
    }

    /**
     * @param state 
     * @param a 
     * @param b 
     * @param condition 
     */
    static void select(u32* state, u32* a, u32* b, u32 condition)
    {
        u32 mask = condition - 1;

        for (auto i = 0; i < WORDS; i++) {
            state[i] = (a[i] & mask) | (b[i] & ~mask);
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
     * @param state 
     * @param first 
     * @param second 
     */
    static void modular_multiply_single(u32* state, u32* first, u32 second)
    {
        u64 temp = 0;
        u64 carry = 0;
        u32 output[WORDS];

        for (auto i = 0; i < WORDS; i++) {
            temp += (u64)first[i] * second;
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        carry = temp;

        for (auto i = 0; i < WORDS / 2; i++) {
            temp += output[i];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        temp += carry;

        for (auto i = WORDS / 2; i < WORDS; i++) {
            temp += output[i];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        modular_reduce(state, output, (u32)temp);
    }

    /**
     * @param state 
     * @param first 
     * @param second 
     */
    static void modular_multiply(u32* state, u32* first, u32* second)
    {
        u64 temp = 0;
        u64 carry = 0;
        u32 output[WORDS * 2];

        for (auto i = 0; i < WORDS * 2; i++) {
            if (i < 14) {
                for (auto j = 0; j <= i; j++) {
                    temp += (u64)first[j] * second[i - j];
                    carry += temp >> 32;
                    temp &= 0xFFFFFFFF;
                }
            } else {
                for (auto j = i - 13; j < WORDS; j++) {
                    temp += (u64)first[j] * second[i - j];
                    carry += temp >> 32;
                    temp &= 0xFFFFFFFF;
                }
            }

            output[i] = temp & 0xFFFFFFFF;
            temp = carry & 0xFFFFFFFF;
            carry >>= 32;
        }

        temp = 0;

        for (auto i = 0; i < WORDS / 2; i++) {
            temp += output[i];
            temp += output[i + 14];
            temp += output[i + 21];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        for (auto i = WORDS / 2; i < WORDS; i++) {
            temp += output[i];
            temp += output[i + 7];
            temp += output[i + 14];
            temp += output[i + 14];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        carry = temp;

        for (auto i = 0; i < WORDS / 2; i++) {
            temp += output[i];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        temp += carry;

        for (auto i = WORDS / 2; i < WORDS; i++) {
            temp += output[i];
            output[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        modular_reduce(state, output, (u32)temp);
    }

    /**
     * @param state 
     * @param value 
     */
    static void modular_square(u32* state, u32* value)
    {
        modular_multiply(state, value, value);
    }

    /**
     * @param state 
     * @param first 
     * @param second 
     */
    static void modular_add(u32* state, u32* first, u32* second)
    {
        u64 temp = 0;

        for (auto i = 0; i < WORDS; i++) {
            temp += first[i];
            temp += second[i];
            state[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        modular_reduce(state, state, (u32)temp);
    }

    /**
     * @param state 
     * @param first 
     * @param second 
     */
    static void modular_subtract(u32* state, u32* first, u32* second)
    {
        i64 temp = -1;

        for (auto i = 0; i < 7; i++) {
            temp += first[i];
            temp -= second[i];
            state[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        temp -= 1;

        for (auto i = 7; i < 14; i++) {
            temp += first[i];
            temp -= second[i];
            state[i] = temp & 0xFFFFFFFF;
            temp >>= 32;
        }

        temp += 1;

        modular_reduce(state, state, (u32)temp);
    }

    /**
     * @param state 
     * @param value 
     * @param n 
     */
    static void to_power_of_2n(u32* state, u32* value, u8 n)
    {
        modular_square(state, value);
        for (auto i = 1; i < n; i++) {
            modular_square(state, state);
        }
    }

    /**
     * @param state 
     * @param value 
     */
    static void modular_multiply_inverse(u32* state, u32* value)
    {
        u32 u[WORDS];
        u32 v[WORDS];

        modular_square(u, value);
        modular_multiply(u, u, value);
        modular_square(u, u);
        modular_multiply(v, u, value);
        to_power_of_2n(u, v, 3);
        modular_multiply(v, u, v);
        to_power_of_2n(u, v, 6);
        modular_multiply(u, u, v);
        modular_square(u, u);
        modular_multiply(v, u, value);
        to_power_of_2n(u, v, 13);
        modular_multiply(u, u, v);
        modular_square(u, u);
        modular_multiply(v, u, value);
        to_power_of_2n(u, v, 27);
        modular_multiply(u, u, v);
        modular_square(u, u);
        modular_multiply(v, u, value);
        to_power_of_2n(u, v, 55);
        modular_multiply(u, u, v);
        modular_square(u, u);
        modular_multiply(v, u, value);
        to_power_of_2n(u, v, 111);
        modular_multiply(v, u, v);
        modular_square(u, v);
        modular_multiply(u, u, value);
        to_power_of_2n(u, u, 223);
        modular_multiply(u, u, v);
        modular_square(u, u);
        modular_square(u, u);
        modular_multiply(state, u, value);
    }

    /**
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> X448::generate_private_key()
    {
        auto buffer = TRY(ByteBuffer::create_uninitialized(BYTES));
        fill_with_random(buffer.data(), buffer.size());
        return buffer;
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

    /**
     * @param input_k 
     * @param input_u 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> X448::compute_coordinate(ReadonlyBytes input_k, ReadonlyBytes input_u)
    {
        u32 k[WORDS] {};
        u32 u[WORDS] {};
        u32 x1[WORDS] {};
        u32 x2[WORDS] {};
        u32 z1[WORDS] {};
        u32 z2[WORDS] {};
        u32 t1[WORDS] {};
        u32 t2[WORDS] {};

        import_state(k, input_k);

        k[0] &= 0xFFFFFFFC;
        k[13] |= 0x80000000;

        import_state(u, input_u);

        modular_reduce(u, u, 0);

        set(x1, 1);
        set(z1, 0);
        copy(x2, u);
        set(z2, 1);

        u32 swap = 0;

        for (auto i = BITS - 1; i >= 0; i--) {
            u32 b = (k[i / 32] >> (i % 32)) & 1;

            conditional_swap(x1, x2, swap ^ b);
            conditional_swap(z1, z2, swap ^ b);

            swap = b;

            modular_add(t1, x2, z2);
            modular_subtract(x2, x2, z2);
            modular_add(z2, x1, z1);
            modular_subtract(x1, x1, z1);
            modular_multiply(t1, t1, x1);
            modular_multiply(x2, x2, z2);
            modular_square(z2, z2);
            modular_square(x1, x1);
            modular_subtract(t2, z2, x1);
            modular_multiply_single(z1, t2, A24);
            modular_add(z1, z1, x1);
            modular_multiply(z1, z1, t2);
            modular_multiply(x1, x1, z2);
            modular_subtract(z2, t1, x2);
            modular_square(z2, z2);
            modular_multiply(z2, z2, u);
            modular_add(x2, x2, t1);
            modular_square(x2, x2);
        }

        conditional_swap(x1, x2, swap);
        conditional_swap(z1, z2, swap);

        modular_multiply_inverse(u, z1);
        modular_multiply(u, u, x1);

        return export_state(u);
    }

    /**
     * @param shared_point 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> X448::derive_premaster_key(ReadonlyBytes shared_point)
    {
        VERIFY(shared_point.size() == BYTES);
        ByteBuffer premaster_key = TRY(ByteBuffer::copy(shared_point));
        return premaster_key;
    }

} // namepsace Crypto::Curves
