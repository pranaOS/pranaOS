/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "UnsignedBigIntegerAlgorithms.h"

namespace Crypto {

FLATTEN void UnsignedBigIntegerAlgorithms::multiply_without_allocation(
    UnsignedBigInteger const& left,
    UnsignedBigInteger const& right,
    UnsignedBigInteger& temp_shift_result,
    UnsignedBigInteger& temp_shift_plus,
    UnsignedBigInteger& temp_shift,
    UnsignedBigInteger& output)
{
    output.set_to_0();

    for (size_t word_index = 0; word_index < left.length(); ++word_index) {
        for (size_t bit_index = 0; bit_index < UnsignedBigInteger::BITS_IN_WORD; ++bit_index) {

            if (!(left.m_words[word_index] & (1 << bit_index)))
                continue;

            size_t shift_amount = word_index * UnsignedBigInteger::BITS_IN_WORD + bit_index;

            shift_left_without_allocation(right, shift_amount, temp_shift_result, temp_shift_plus, temp_shift);
            add_into_accumulator_without_allocation(output, temp_shift);
        }
    }
}

}
