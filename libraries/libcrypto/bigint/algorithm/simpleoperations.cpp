/**
 * @file simpleoperations.cpp
 * @author Krisna Pranav
 * @brief Simple Operations
 * @version 6.0
 * @date 2024-11-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "unsignedbigintegeralgorithms.h"

namespace Crypto 
{

    /**
     * @param left 
     * @param right 
     * @param output 
     */
    void UnsignedBigIntegerAlgorithms::add_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output)
    {
        const UnsignedBigInteger* const longer = (left.length() > right.length()) ? &left : &right;
        const UnsignedBigInteger* const shorter = (longer == &right) ? &left : &right;

        output.set_to(*longer);
        add_into_accumulator_without_allocation(output, *shorter);
    }

    /**
     * @param accumulator 
     * @param value 
     */
    void UnsignedBigIntegerAlgorithms::add_into_accumulator_without_allocation(UnsignedBigInteger& accumulator, UnsignedBigInteger const& value)
    {
        auto value_length = value.trimmed_length();

        accumulator.resize_with_leading_zeros(value_length);
        auto final_length = accumulator.length();

        UnsignedBigInteger::Word last_carry_for_word = 0;

        for (size_t i = 0; i < value_length; ++i) {
            UnsignedBigInteger::Word current_carry_for_word = 0;

            if (Checked<UnsignedBigInteger::Word>::addition_would_overflow(value.m_words[i], accumulator.m_words[i])) {
                current_carry_for_word = 1;
            }
            UnsignedBigInteger::Word word_addition_result = value.m_words[i] + accumulator.m_words[i];
            if (Checked<UnsignedBigInteger::Word>::addition_would_overflow(word_addition_result, last_carry_for_word)) {
                current_carry_for_word = 1;
            }
            word_addition_result += last_carry_for_word;
            last_carry_for_word = current_carry_for_word;
            accumulator.m_words[i] = word_addition_result;
        }

        while (last_carry_for_word && final_length > value_length) {
            UnsignedBigInteger::Word current_carry_for_word = 0;
            if (Checked<UnsignedBigInteger::Word>::addition_would_overflow(accumulator.m_words[value_length], last_carry_for_word)) {
                current_carry_for_word = 1;
            }
            accumulator.m_words[value_length] += last_carry_for_word;
            last_carry_for_word = current_carry_for_word;
            value_length++;
        }

        if (last_carry_for_word) {
            accumulator.m_words.append(last_carry_for_word);
        }
    }

    /**
     * @param left 
     * @param right 
     * @param output 
     */
    void UnsignedBigIntegerAlgorithms::subtract_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output)
    {
        if (left < right) {
            output.invalidate();
            return;
        }

        u8 borrow = 0;
        auto own_length = left.length();
        auto other_length = right.length();

        output.set_to_0();
        output.m_words.resize_and_keep_capacity(own_length);

        for (size_t i = 0; i < own_length; ++i) {
            u32 other_word = (i < other_length) ? right.m_words[i] : 0;
            i64 temp = static_cast<i64>(left.m_words[i]) - static_cast<i64>(other_word) - static_cast<i64>(borrow);
            borrow = (temp >= 0) ? 0 : 1;
            if (temp < 0) {
                temp += (UINT32_MAX + 1);
            }
            output.m_words[i] = temp;
        }

        VERIFY(borrow == 0);
    }

} // namespace Crypto