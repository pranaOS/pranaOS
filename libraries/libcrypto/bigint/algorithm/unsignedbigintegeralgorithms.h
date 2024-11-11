/**
 * @file unsignedbigintegeralgorithms.h
 * @author Krisna Pranav
 * @brief Unsigned Big Integer Alogrithms
 * @version 6.0
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto 
{
    class UnsignedBigIntegerAlgorithms 
    {
    public:
        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void add_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output);

        /**
         * @param accumulator 
         * @param value 
         */
        static void add_into_accumulator_without_allocation(UnsignedBigInteger& accumulator, UnsignedBigInteger const& value);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void subtract_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_or_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_and_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_xor_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param output 
         */
        static void bitwise_not_fill_to_one_based_index_without_allocation(UnsignedBigInteger const& left, size_t, UnsignedBigInteger& output);

        /**
         * @param number 
         * @param bits_to_shift_by 
         * @param temp_result 
         * @param temp_plus 
         * @param output 
         */
        static void shift_left_without_allocation(UnsignedBigInteger const& number, size_t bits_to_shift_by, UnsignedBigInteger& temp_result, UnsignedBigInteger& temp_plus, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param temp_shift_result 
         * @param temp_shift_plus 
         * @param temp_shift 
         * @param output 
         */
        static void multiply_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& temp_shift_result, UnsignedBigInteger& temp_shift_plus, UnsignedBigInteger& temp_shift, UnsignedBigInteger& output);

        /**
         * @param numerator 
         * @param denominator 
         * @param temp_shift_result 
         * @param temp_shift_plus 
         * @param temp_shift 
         * @param temp_minus 
         * @param quotient 
         * @param remainder 
         */
        static void divide_without_allocation(UnsignedBigInteger const& numerator, UnsignedBigInteger const& denominator, UnsignedBigInteger& temp_shift_result, UnsignedBigInteger& temp_shift_plus, UnsignedBigInteger& temp_shift, UnsignedBigInteger& temp_minus, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder);

        /**
         * @param numerator 
         * @param denominator 
         * @param quotient 
         * @param remainder 
         */
        static void divide_u16_without_allocation(UnsignedBigInteger const& numerator, UnsignedBigInteger::Word denominator, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder);

        /**
         * @param temp_a 
         * @param temp_b 
         * @param temp_1 
         * @param temp_2 
         * @param temp_3 
         * @param temp_4 
         * @param temp_quotient 
         * @param temp_remainder 
         * @param output 
         */
        static void destructive_GCD_without_allocation(UnsignedBigInteger& temp_a, UnsignedBigInteger& temp_b, UnsignedBigInteger& temp_1, UnsignedBigInteger& temp_2, UnsignedBigInteger& temp_3, UnsignedBigInteger& temp_4, UnsignedBigInteger& temp_quotient, UnsignedBigInteger& temp_remainder, UnsignedBigInteger& output);

        /**
         * @param a_ 
         * @param b 
         * @param temp_1 
         * @param temp_2 
         * @param temp_3 
         * @param temp_4 
         * @param temp_minus 
         * @param temp_quotient 
         * @param temp_d 
         * @param temp_u 
         * @param temp_v 
         * @param temp_x 
         * @param result 
         */
        static void modular_inverse_without_allocation(UnsignedBigInteger const& a_, UnsignedBigInteger const& b, UnsignedBigInteger& temp_1, UnsignedBigInteger& temp_2, UnsignedBigInteger& temp_3, UnsignedBigInteger& temp_4, UnsignedBigInteger& temp_minus, UnsignedBigInteger& temp_quotient, UnsignedBigInteger& temp_d, UnsignedBigInteger& temp_u, UnsignedBigInteger& temp_v, UnsignedBigInteger& temp_x, UnsignedBigInteger& result);

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
         * @param result 
         */
        static void destructive_modular_power_without_allocation(UnsignedBigInteger& ep, UnsignedBigInteger& base, UnsignedBigInteger const& m, UnsignedBigInteger& temp_1, UnsignedBigInteger& temp_2, UnsignedBigInteger& temp_3, UnsignedBigInteger& temp_4, UnsignedBigInteger& temp_multiply, UnsignedBigInteger& temp_quotient, UnsignedBigInteger& temp_remainder, UnsignedBigInteger& result);

        /**
         * @param base 
         * @param exponent 
         * @param modulo 
         * @param temp_z0 
         * @param temp_rr 
         * @param temp_one 
         * @param temp_z 
         * @param temp_zz 
         * @param temp_x 
         * @param temp_extra 
         * @param result 
         */
        static void montgomery_modular_power_with_minimal_allocations(UnsignedBigInteger const& base, UnsignedBigInteger const& exponent, UnsignedBigInteger const& modulo, UnsignedBigInteger& temp_z0, UnsignedBigInteger& temp_rr, UnsignedBigInteger& temp_one, UnsignedBigInteger& temp_z, UnsignedBigInteger& temp_zz, UnsignedBigInteger& temp_x, UnsignedBigInteger& temp_extra, UnsignedBigInteger& result);

    private:

        /**
         * @param z 
         * @param offset_in_z 
         * @param x 
         * @param y_digit 
         * @param num_words 
         * @return UnsignedBigInteger::Word 
         */
        static UnsignedBigInteger::Word montgomery_fragment(UnsignedBigInteger& z, size_t offset_in_z, UnsignedBigInteger const& x, UnsignedBigInteger::Word y_digit, size_t num_words);

        /**
         * @param x 
         * @param y 
         * @param modulo 
         * @param z 
         * @param k 
         * @param num_words 
         * @param result 
         */
        static void almost_montgomery_multiplication_without_allocation(UnsignedBigInteger const& x, UnsignedBigInteger const& y, UnsignedBigInteger const& modulo, UnsignedBigInteger& z, UnsignedBigInteger::Word k, size_t num_words, UnsignedBigInteger& result);

        /**
         * @param number 
         * @param number_of_words 
         * @param output 
         */
        static void shift_left_by_n_words(UnsignedBigInteger const& number, size_t number_of_words, UnsignedBigInteger& output);

        /**
         * @param number 
         * @param number_of_words 
         * @param output 
         */
        static void shift_right_by_n_words(UnsignedBigInteger const& number, size_t number_of_words, UnsignedBigInteger& output);

        /**
         * @param number 
         * @param num_bits 
         * @param result_word_index 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static UnsignedBigInteger::Word shift_left_get_one_word(UnsignedBigInteger const& number, size_t num_bits, size_t result_word_index);
    }; // class UnsignedBigIntegerAlgorithms
} // namespace Crypto
