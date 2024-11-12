/**
 * @file division.cpp
 * @author Krisna Pranav
 * @brief Division
 * @version 6.0
 * @date 2024-11-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "unsignedbigintegeralgorithms.h"

namespace Crypto 
{
    /**
     * @param numerator 
     * @param denominator 
     * @param temp_shift_result 
     * @param temp_shift_plus 
     * @param temp_shift 
     * @param temp_minus 
     * @param quotient 
     * @param remainder 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::divide_without_allocation(UnsignedBigInteger const& numerator, UnsignedBigInteger const& denominator, UnsignedBigInteger& temp_shift_result, UnsignedBigInteger& temp_shift_plus, UnsignedBigInteger& temp_shift, UnsignedBigInteger& temp_minus, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder)
    {
        quotient.set_to_0();
        remainder.set_to(numerator);

        for (int word_index = numerator.trimmed_length() - 1; word_index >= 0; --word_index) {
            for (int bit_index = UnsignedBigInteger::BITS_IN_WORD - 1; bit_index >= 0; --bit_index) {
                size_t shift_amount = word_index * UnsignedBigInteger::BITS_IN_WORD + bit_index;
                shift_left_without_allocation(denominator, shift_amount, temp_shift_result, temp_shift_plus, temp_shift);

                subtract_without_allocation(remainder, temp_shift, temp_minus);
                if (!temp_minus.is_invalid()) {
                    remainder.set_to(temp_minus);
                    quotient.set_bit_inplace(shift_amount);
                }
            }
        }
    }

    /**
     * @param numerator 
     * @param denominator 
     * @param quotient 
     * @param remainder 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::divide_u16_without_allocation(UnsignedBigInteger const& numerator, UnsignedBigInteger::Word denominator, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder)
    {
        VERIFY(denominator < (1 << 16));
        UnsignedBigInteger::Word remainder_word = 0;
        auto numerator_length = numerator.trimmed_length();
        quotient.set_to_0();
        quotient.m_words.resize(numerator_length);
        for (int word_index = numerator_length - 1; word_index >= 0; --word_index) {
            auto word_high = numerator.m_words[word_index] >> 16;
            auto word_low = numerator.m_words[word_index] & ((1 << 16) - 1);

            auto number_to_divide_high = (remainder_word << 16) | word_high;
            auto quotient_high = number_to_divide_high / denominator;
            remainder_word = number_to_divide_high % denominator;

            auto number_to_divide_low = remainder_word << 16 | word_low;
            auto quotient_low = number_to_divide_low / denominator;
            remainder_word = number_to_divide_low % denominator;

            quotient.m_words[word_index] = (quotient_high << 16) | quotient_low;
        }
        remainder.set_to(remainder_word);
    }
} // namespace Crypto