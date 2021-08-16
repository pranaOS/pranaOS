/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "UnsignedBigIntegerAlgorithms.h"

namespace Crypto {

FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_or_without_allocation(
    UnsignedBigInteger const& left,
    UnsignedBigInteger const& right,
    UnsignedBigInteger& output)
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

FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_and_without_allocation(
    UnsignedBigInteger const& left,
    UnsignedBigInteger const& right,
    UnsignedBigInteger& output)
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

FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_xor_without_allocation(
    UnsignedBigInteger const& left,
    UnsignedBigInteger const& right,
    UnsignedBigInteger& output)
{
    // If either of the BigInts are invalid, the output is just the other one.
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
 * Complexity: O(N) where N is the number of words
 */
FLATTEN void UnsignedBigIntegerAlgorithms::bitwise_not_without_allocation(
    UnsignedBigInteger const& right,
    UnsignedBigInteger& output)
{
    // If the value is invalid, the output value is invalid as well.
    if (right.is_invalid()) {
        output.invalidate();
        return;
    }
    if (right.length() == 0) {
        output.set_to_0();
        return;
    }

    output.m_words.resize_and_keep_capacity(right.length());

    if (right.length() > 1) {
        for (size_t i = 0; i < right.length() - 1; ++i)
            output.m_words[i] = ~right.words()[i];
    }

    auto last_word_index = right.length() - 1;
    auto last_word = right.words()[last_word_index];

    output.m_words[last_word_index] = ((u32)0xffffffffffffffff >> __builtin_clz(last_word)) & ~last_word;
}

FLATTEN void UnsignedBigIntegerAlgorithms::shift_left_without_allocation(
    UnsignedBigInteger const& number,
    size_t num_bits,
    UnsignedBigInteger& temp_result,
    UnsignedBigInteger& temp_plus,
    UnsignedBigInteger& output)
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

void UnsignedBigIntegerAlgorithms::shift_left_by_n_words(
    UnsignedBigInteger const& number,
    size_t number_of_words,
    UnsignedBigInteger& output)
{
    output.set_to_0();
    output.m_words.resize_and_keep_capacity(number_of_words + number.length());

    __builtin_memset(output.m_words.data(), 0, number_of_words * sizeof(unsigned));
    __builtin_memcpy(&output.m_words.data()[number_of_words], number.m_words.data(), number.m_words.size() * sizeof(unsigned));
}

void UnsignedBigIntegerAlgorithms::shift_right_by_n_words(
    UnsignedBigInteger const& number,
    size_t number_of_words,
    UnsignedBigInteger& output)
{
    output.set_to_0();
    output.m_words.resize_and_keep_capacity(number.length() - number_of_words);
    __builtin_memcpy(output.m_words.data(), &number.m_words.data()[number_of_words], (number.m_words.size() - number_of_words) * sizeof(unsigned));
}

ALWAYS_INLINE UnsignedBigInteger::Word UnsignedBigIntegerAlgorithms::shift_left_get_one_word(
    UnsignedBigInteger const& number,
    size_t num_bits,
    size_t result_word_index)
{
    // "<= length()" (rather than length() - 1) is intentional,
    // The result inedx of length() is used when calculating the carry word
    VERIFY(result_word_index <= number.length());
    VERIFY(num_bits <= UnsignedBigInteger::BITS_IN_WORD);
    u32 result = 0;

    // we need to check for "num_bits != 0" since shifting right by 32 is apparently undefined behaviour!
    if (result_word_index > 0 && num_bits != 0) {
        result += number.m_words[result_word_index - 1] >> (UnsignedBigInteger::BITS_IN_WORD - num_bits);
    }
    if (result_word_index < number.length() && num_bits < 32) {
        result += number.m_words[result_word_index] << num_bits;
    }
    return result;
}

}
