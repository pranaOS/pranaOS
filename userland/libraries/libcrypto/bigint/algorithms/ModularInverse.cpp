/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "UnsignedBigIntegerAlgorithms.h"

namespace Crypto {

void UnsignedBigIntegerAlgorithms::modular_inverse_without_allocation(
    UnsignedBigInteger const& a,
    UnsignedBigInteger const& b,
    UnsignedBigInteger& temp_1,
    UnsignedBigInteger& temp_2,
    UnsignedBigInteger& temp_3,
    UnsignedBigInteger& temp_4,
    UnsignedBigInteger& temp_minus,
    UnsignedBigInteger& temp_quotient,
    UnsignedBigInteger& temp_d,
    UnsignedBigInteger& temp_u,
    UnsignedBigInteger& temp_v,
    UnsignedBigInteger& temp_x,
    UnsignedBigInteger& result)
{
    UnsignedBigInteger one { 1 };

    temp_u.set_to(a);
    if (a.words()[0] % 2 == 0) {

        add_into_accumulator_without_allocation(temp_u, b);
    }

    temp_v.set_to(b);
    temp_x.set_to(0);

    subtract_without_allocation(b, one, temp_d);

    while (!(temp_v == 1)) {
        while (temp_v < temp_u) {

            subtract_without_allocation(temp_u, temp_v, temp_minus);
            temp_u.set_to(temp_minus);

            add_into_accumulator_without_allocation(temp_d, temp_x);

            while (temp_u.words()[0] % 2 == 0) {
                if (temp_d.words()[0] % 2 == 1) {
                   
                    add_into_accumulator_without_allocation(temp_d, b);
                }

                divide_u16_without_allocation(temp_u, 2, temp_quotient, temp_1);
                temp_u.set_to(temp_quotient);


                divide_u16_without_allocation(temp_d, 2, temp_quotient, temp_1);
                temp_d.set_to(temp_quotient);
            }
        }

        subtract_without_allocation(temp_v, temp_u, temp_minus);
        temp_v.set_to(temp_minus);

        add_into_accumulator_without_allocation(temp_x, temp_d);

        while (temp_v.words()[0] % 2 == 0) {
            if (temp_x.words()[0] % 2 == 1) {
            
                add_into_accumulator_without_allocation(temp_x, b);
            }

            divide_u16_without_allocation(temp_v, 2, temp_quotient, temp_1);
            temp_v.set_to(temp_quotient);

            divide_u16_without_allocation(temp_x, 2, temp_quotient, temp_1);
            temp_x.set_to(temp_quotient);
        }
    }


    divide_without_allocation(temp_x, b, temp_1, temp_2, temp_3, temp_4, temp_quotient, result);
}

}
