/**
 * @file gcd.cpp
 * @author Krisna Pranav
 * @brief GCD
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
    void UnsignedBigIntegerAlgorithms::destructive_GCD_without_allocation(UnsignedBigInteger& temp_a, UnsignedBigInteger& temp_b, UnsignedBigInteger& temp_1, UnsignedBigInteger& temp_2, UnsignedBigInteger& temp_3, UnsignedBigInteger& temp_4, UnsignedBigInteger& temp_quotient, UnsignedBigInteger& temp_remainder, UnsignedBigInteger& output)
    {
        for (;;) {
            if (temp_a == 0) {
                output.set_to(temp_b);
                return;
            }

            divide_without_allocation(temp_b, temp_a, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder);
            temp_b.set_to(temp_remainder);
            if (temp_b == 0) {
                output.set_to(temp_a);
                return;
            }

            divide_without_allocation(temp_a, temp_b, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder);
            temp_a.set_to(temp_remainder);
        }
    }
} // namespace Crypto