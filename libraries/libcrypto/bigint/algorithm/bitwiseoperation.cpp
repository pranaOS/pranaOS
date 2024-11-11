/**
 * @file bitwiseoperation.cpp
 * @author Krisna Pranav
 * @brief BitWise Operation
 * @version 6.0
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "unsignedbigintegeralgorithms.h"
#include <mods/builtinwrappers.h>
#include <mods/numericlimits.h>

namespace Crypto 
{

    /**
     * @param left 
     * @param right 
     * @param output 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_or_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output)
    {
        if (left.is_invalid()) {
            output.set_to(right);
            return;
        }
        if (right.is_invalid()) {
            output.set_to(left);
            return;
        }

        const UnsignedBigInteger *shorter, *longer;
        if (left.length() < right.length()) {
            shorter = &left;
            longer = &right;
        } else {
            shorter = &right;
            longer = &left;
        }

        output.m_words.resize_and_keep_capacity(longer->length());

        size_t longer_offset = longer->length() - shorter->length();
        for (size_t i = 0; i < shorter->length(); ++i)
            output.m_words[i] = longer->words()[i] | shorter->words()[i];

        __builtin_memcpy(output.m_words.data() + shorter->length(), longer->words().data() + shorter->length(), sizeof(u32) * longer_offset);
    }

    /**
     * @param left 
     * @param right 
     * @param output 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_and_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output)
    {
        if (left.is_invalid()) {
            output.set_to(right);
            return;
        }

        if (right.is_invalid()) {
            output.set_to(left);
            return;
        }

        const UnsignedBigInteger *shorter, *longer;

        if (left.length() < right.length()) {
            shorter = &left;
            longer = &right;
        } else {
            shorter = &right;
            longer = &left;
        }

        output.m_words.resize_and_keep_capacity(longer->length());

        size_t longer_offset = longer->length() - shorter->length();
        for (size_t i = 0; i < shorter->length(); ++i)
            output.m_words[i] = longer->words()[i] & shorter->words()[i];

        __builtin_memset(output.m_words.data() + shorter->length(), 0, sizeof(u32) * longer_offset);
    }

    /**
     * @param left 
     * @param right 
     * @param output 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_xor_without_allocation(UnsignedBigInteger const& left, UnsignedBigInteger const& right, UnsignedBigInteger& output)
    {
        if (left.is_invalid()) {
            output.set_to(right);
            return;
        }

        if (right.is_invalid()) {
            output.set_to(left);
            return;
        }

        const UnsignedBigInteger *shorter, *longer;

        if (left.length() < right.length()) {
            shorter = &left;
            longer = &right;
        } else {
            shorter = &right;
            longer = &left;
        }

        output.m_words.resize_and_keep_capacity(longer->length());

        size_t longer_offset = longer->length() - shorter->length();
        for (size_t i = 0; i < shorter->length(); ++i)
            output.m_words[i] = longer->words()[i] ^ shorter->words()[i];

        __builtin_memcpy(output.m_words.data() + shorter->length(), longer->words().data() + shorter->length(), sizeof(u32) * longer_offset);
    }

    /**
     * @param right 
     * @param index 
     * @param output 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_not_fill_to_one_based_index_without_allocation(UnsignedBigInteger const& right, size_t index, UnsignedBigInteger& output)
    {
        if (right.is_invalid()) {
            output.invalidate();
            return;
        }

        if (index == 0) {
            output.set_to_0();
            return;
        }

        size_t size = (index + UnsignedBigInteger::BITS_IN_WORD - 1) / UnsignedBigInteger::BITS_IN_WORD;

        output.m_words.resize_and_keep_capacity(size);
        VERIFY(size > 0);
        for (size_t i = 0; i < size - 1; ++i)
            output.m_words[i] = ~(i < right.length() ? right.words()[i] : 0);

        index -= (size - 1) * UnsignedBigInteger::BITS_IN_WORD;
        auto last_word_index = size - 1;
        auto last_word = last_word_index < right.length() ? right.words()[last_word_index] : 0;

        output.m_words[last_word_index] = (NumericLimits<UnsignedBigInteger::Word>::max() >> (UnsignedBigInteger::BITS_IN_WORD - index)) & ~last_word;
    }

    /**
     * @param number 
     * @param num_bits 
     * @param temp_result 
     * @param temp_plus 
     * @param output 
     * @return FLATTEN 
     */
    FLATTEN void UnsignedBigIntegerAlgorithms::shift_left_without_allocation(UnsignedBigInteger const& number, size_t num_bits, UnsignedBigInteger& temp_result, UnsignedBigInteger& temp_plus, UnsignedBigInteger& output)
    {
        shift_left_by_n_words(number, num_bits / UnsignedBigInteger::BITS_IN_WORD, temp_result);

        output.set_to(temp_result);

        num_bits %= UnsignedBigInteger::BITS_IN_WORD;

        if (num_bits == 0) {
            return;
        }

        for (size_t i = 0; i < temp_result.length(); ++i) {
            u32 current_word_of_temp_result = shift_left_get_one_word(temp_result, num_bits, i);
            output.m_words[i] = current_word_of_temp_result;
        }

        u32 carry_word = shift_left_get_one_word(temp_result, num_bits, temp_result.length());
        if (carry_word != 0) {
            temp_plus.set_to_0();
            temp_plus.m_words.append(carry_word);
            shift_left_by_n_words(temp_plus, temp_result.length(), temp_result);
            add_into_accumulator_without_allocation(output, temp_result);
        }
    }

    /**
     * @param number 
     * @param number_of_words 
     * @param output 
     */
    void UnsignedBigIntegerAlgorithms::shift_left_by_n_words(UnsignedBigInteger const& number, size_t number_of_words, UnsignedBigInteger& output)
    {
        output.set_to_0();
        output.m_words.resize_and_keep_capacity(number_of_words + number.length());

        __builtin_memset(output.m_words.data(), 0, number_of_words * sizeof(unsigned));
        __builtin_memcpy(&output.m_words.data()[number_of_words], number.m_words.data(), number.m_words.size() * sizeof(unsigned));
    }

    /**
     * @param number 
     * @param number_of_words 
     * @param output 
     */
    void UnsignedBigIntegerAlgorithms::shift_right_by_n_words(UnsignedBigInteger const& number,size_t number_of_words, UnsignedBigInteger& output)
    {
        output.set_to_0();
        output.m_words.resize_and_keep_capacity(number.length() - number_of_words);
        __builtin_memcpy(output.m_words.data(), &number.m_words.data()[number_of_words], (number.m_words.size() - number_of_words) * sizeof(unsigned));
    }

    /**
     * @param number 
     * @param num_bits 
     * @param result_word_index 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE UnsignedBigInteger::Word UnsignedBigIntegerAlgorithms::shift_left_get_one_word(UnsignedBigInteger const& number, size_t num_bits, size_t result_word_index)
    {
        VERIFY(result_word_index <= number.length());
        VERIFY(num_bits <= UnsignedBigInteger::BITS_IN_WORD);
        u32 result = 0;

        if (result_word_index > 0 && num_bits != 0) {
            result += number.m_words[result_word_index - 1] >> (UnsignedBigInteger::BITS_IN_WORD - num_bits);
        }

        if (result_word_index < number.length() && num_bits < 32) {
            result += number.m_words[result_word_index] << num_bits;
        }

        return result;
    }
} // namespace Crypto