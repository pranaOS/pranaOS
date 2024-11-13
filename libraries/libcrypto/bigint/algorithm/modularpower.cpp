/**
 * @file modularpower.cpp
 * @author Krisna Pranav
 * @brief Modular Power
 * @version 6.0
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "unsignedbigintegeralgorithms.h"

namespace Crypto 
{
    /**
     * @param ep 
     * @param base 
     * @param m 
     * @param temp_1 
     * @param temp_2 
     * @param temp_3 
     * @param temp_4 
     * @param temp_multiply 
     * @param temp_quotient 
     * @param temp_remainder 
     * @param exp 
     */
    void UnsignedBigIntegerAlgorithms::destructive_modular_power_without_allocation(UnsignedBigInteger& ep, UnsignedBigInteger& base, UnsignedBigInteger const& m, UnsignedBigInteger& temp_1, UnsignedBigInteger& temp_2, UnsignedBigInteger& temp_3, UnsignedBigInteger& temp_4, UnsignedBigInteger& temp_multiply, UnsignedBigInteger& temp_quotient, UnsignedBigInteger& temp_remainder, UnsignedBigInteger& exp)
    {
        exp.set_to(1);
        while (!(ep < 1)) {
            if (ep.words()[0] % 2 == 1) {
                multiply_without_allocation(exp, base, temp_1, temp_2, temp_3, temp_multiply);
                divide_without_allocation(temp_multiply, m, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder);
                exp.set_to(temp_remainder);
            }

            divide_u16_without_allocation(ep, 2, temp_quotient, temp_remainder);
            ep.set_to(temp_quotient);

            multiply_without_allocation(base, base, temp_1, temp_2, temp_3, temp_multiply);
            divide_without_allocation(temp_multiply, m, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder);
            base.set_to(temp_remainder);

            ep.clamp_to_trimmed_length();
            exp.clamp_to_trimmed_length();
            base.clamp_to_trimmed_length();
        }
    }

    /**
     * @param value 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static u32 inverse_wrapped(u32 value)
    {
        VERIFY(value & 1);

        u64 b = static_cast<u64>(value);
        u64 k0 = (2 - b);
        u64 t = (b - 1);
        size_t i = 1;
        while (i < 32) {
            t = t * t;
            k0 = k0 * (t + 1);
            i <<= 1;
        }
        return static_cast<u32>(-k0);
    }

    /**
     * @param x 
     * @param y 
     * @param c 
     * @param z_carry 
     * @param z 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static void linear_multiplication_with_carry(u32 x, u32 y, u32 c, u32& z_carry, u32& z)
    {
        u64 result = static_cast<u64>(x) * static_cast<u64>(y) + static_cast<u64>(c);
        z_carry = static_cast<u32>(result >> 32);
        z = static_cast<u32>(result);
    }

    /**
     * @param a 
     * @param b 
     * @param z_carry 
     * @param z 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static void addition_with_carry(u32 a, u32 b, u32& z_carry, u32& z)
    {
        u64 result = static_cast<u64>(a) + static_cast<u64>(b);
        z_carry = static_cast<u32>(result >> 32);
        z = static_cast<u32>(result);
    }

    /**
     * @param z 
     * @param offset_in_z 
     * @param x 
     * @param y_digit 
     * @param num_words 
     * @return UnsignedBigInteger::Word 
     */
    UnsignedBigInteger::Word UnsignedBigIntegerAlgorithms::montgomery_fragment(UnsignedBigInteger& z, size_t offset_in_z, UnsignedBigInteger const& x, UnsignedBigInteger::Word y_digit, size_t num_words)
    {
        UnsignedBigInteger::Word carry { 0 };
        for (size_t i = 0; i < num_words; ++i) {
            UnsignedBigInteger::Word a_carry;
            UnsignedBigInteger::Word a;
            linear_multiplication_with_carry(x.m_words[i], y_digit, z.m_words[offset_in_z + i], a_carry, a);
            UnsignedBigInteger::Word b_carry;
            UnsignedBigInteger::Word b;
            addition_with_carry(a, carry, b_carry, b);
            z.m_words[offset_in_z + i] = b;
            carry = a_carry + b_carry;
        }
        return carry;
    }

    /**
     * @param x 
     * @param y 
     * @param modulo 
     * @param z 
     * @param k 
     * @param num_words 
     * @param result 
     */
    void UnsignedBigIntegerAlgorithms::almost_montgomery_multiplication_without_allocation(UnsignedBigInteger const& x, UnsignedBigInteger const& y, UnsignedBigInteger const& modulo, UnsignedBigInteger& z, UnsignedBigInteger::Word k, size_t num_words, UnsignedBigInteger& result)
    {
        VERIFY(x.length() >= num_words);
        VERIFY(y.length() >= num_words);
        VERIFY(modulo.length() >= num_words);

        z.set_to(0);
        z.resize_with_leading_zeros(num_words * 2);

        UnsignedBigInteger::Word previous_double_carry { 0 };
        for (size_t i = 0; i < num_words; ++i) {
            UnsignedBigInteger::Word carry_1 = montgomery_fragment(z, i, x, y.m_words[i], num_words);

            UnsignedBigInteger::Word t = z.m_words[i] * k;
            UnsignedBigInteger::Word carry_2 = montgomery_fragment(z, i, modulo, t, num_words);

            UnsignedBigInteger::Word temp_carry = previous_double_carry + carry_1;
            UnsignedBigInteger::Word overall_carry = temp_carry + carry_2;
            z.m_words[num_words + i] = overall_carry;

            previous_double_carry = (temp_carry < carry_1 || overall_carry < carry_2) ? 1 : 0;
        }

        if (previous_double_carry == 0) {
            shift_right_by_n_words(z, num_words, result);
            result.resize_with_leading_zeros(num_words);
            return;
        }

        UnsignedBigInteger::Word c { 0 };
        for (size_t i = 0; i < num_words; ++i) {
            UnsignedBigInteger::Word z_digit = z.m_words[num_words + i];
            UnsignedBigInteger::Word modulo_digit = modulo.m_words[i];
            UnsignedBigInteger::Word new_z_digit = z_digit - modulo_digit - c;
            z.m_words[i] = new_z_digit;

            c = ((modulo_digit & ~z_digit) | ((modulo_digit | ~z_digit) & new_z_digit)) >> (UnsignedBigInteger::BITS_IN_WORD - 1);
        }

        z.m_words.resize(num_words);
        result.set_to(z);
        result.resize_with_leading_zeros(num_words);
    }

    /**
     * @param base 
     * @param exponent 
     * @param modulo 
     * @param temp_z 
     * @param rr 
     * @param one 
     * @param z 
     * @param zz 
     * @param x 
     * @param temp_extra 
     * @param result 
     */
    void UnsignedBigIntegerAlgorithms::montgomery_modular_power_with_minimal_allocations(UnsignedBigInteger const& base, UnsignedBigInteger const& exponent, UnsignedBigInteger const& modulo, UnsignedBigInteger& temp_z, UnsignedBigInteger& rr, UnsignedBigInteger& one, UnsignedBigInteger& z, UnsignedBigInteger& zz, UnsignedBigInteger& x, UnsignedBigInteger& temp_extra, UnsignedBigInteger& result)
    {
        VERIFY(modulo.is_odd());

        constexpr size_t window_size = 4;

        size_t num_words = modulo.trimmed_length();
        UnsignedBigInteger::Word k = inverse_wrapped(modulo.m_words[0]);

        one.set_to(1);

        shift_left_by_n_words(one, 2 * num_words, x);
        divide_without_allocation(x, modulo, temp_z, one, z, zz, temp_extra, rr);
        rr.resize_with_leading_zeros(num_words);

        x.set_to(base);

        if (x.trimmed_length() > num_words) {
            divide_without_allocation(base, modulo, temp_z, one, z, zz, temp_extra, x);
        }

        x.resize_with_leading_zeros(num_words);

        one.set_to(1);
        one.resize_with_leading_zeros(num_words);

        UnsignedBigInteger powers[1 << window_size];
        almost_montgomery_multiplication_without_allocation(one, rr, modulo, temp_z, k, num_words, powers[0]);
        almost_montgomery_multiplication_without_allocation(x, rr, modulo, temp_z, k, num_words, powers[1]);
        for (size_t i = 2; i < (1 << window_size); ++i)
            almost_montgomery_multiplication_without_allocation(powers[i - 1], powers[1], modulo, temp_z, k, num_words, powers[i]);

        z.set_to(powers[0]);
        z.resize_with_leading_zeros(num_words);
        zz.set_to(0);
        zz.resize_with_leading_zeros(num_words);

        ssize_t exponent_length = exponent.trimmed_length();
        for (ssize_t word_in_exponent = exponent_length - 1; word_in_exponent >= 0; --word_in_exponent) {
            UnsignedBigInteger::Word exponent_word = exponent.m_words[word_in_exponent];
            size_t bit_in_word = 0;
            while (bit_in_word < UnsignedBigInteger::BITS_IN_WORD) {
                if (word_in_exponent != exponent_length - 1 || bit_in_word != 0) {
                    almost_montgomery_multiplication_without_allocation(z, z, modulo, temp_z, k, num_words, zz);
                    almost_montgomery_multiplication_without_allocation(zz, zz, modulo, temp_z, k, num_words, z);
                    almost_montgomery_multiplication_without_allocation(z, z, modulo, temp_z, k, num_words, zz);
                    almost_montgomery_multiplication_without_allocation(zz, zz, modulo, temp_z, k, num_words, z);
                }
                auto power_index = exponent_word >> (UnsignedBigInteger::BITS_IN_WORD - window_size);
                auto& power = powers[power_index];
                almost_montgomery_multiplication_without_allocation(z, power, modulo, temp_z, k, num_words, zz);

                swap(z, zz);

                exponent_word <<= window_size;
                bit_in_word += window_size;
            }
        }

        almost_montgomery_multiplication_without_allocation(z, one, modulo, temp_z, k, num_words, zz);

        if (zz < modulo) {
            result.set_to(zz);
            result.clamp_to_trimmed_length();
            return;
        }

        subtract_without_allocation(zz, modulo, result);

        if (modulo < zz) {
            dbgln("Encountered the branch during a modular power. Params : {} - {} - {}", base, exponent, modulo);
            divide_without_allocation(zz, modulo, temp_z, rr, z, x, temp_extra, result);
        }

        result.clamp_to_trimmed_length();
        return;
    }

} // namespace Crypto