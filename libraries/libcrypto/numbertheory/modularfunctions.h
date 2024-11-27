/**
 * @file modularfunctions.h
 * @author Krisna Pranav
 * @brief Modular Functions
 * @version 6.0
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/random.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto 
{
    namespace NumberTheory 
    {
        /**
         * @param a_ 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger ModularInverse(const UnsignedBigInteger& a_, const UnsignedBigInteger& b);

        /**
         * @param b 
         * @param e 
         * @param m 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger ModularPower(const UnsignedBigInteger& b, const UnsignedBigInteger& e, const UnsignedBigInteger& m);

        /**
         * @tparam IntegerType 
         * @param b 
         * @param e 
         * @return IntegerType 
         */
        template<typename IntegerType>
        static IntegerType Power(const IntegerType& b, const IntegerType& e)
        {
            IntegerType ep { e };
            IntegerType base { b };
            IntegerType exp { 1 };

            while (!(ep < IntegerType { 1 })) {
                if (ep.words()[0] % 2 == 1) {
                    exp.set_to(exp.multiplied_by(base));
                }

                ep.set_to(ep.divided_by(IntegerType { 2 }).quotient);

                base.set_to(base.multiplied_by(base));
            }

            return exp;
        }

        /**
         * @param a 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger GCD(const UnsignedBigInteger& a, const UnsignedBigInteger& b);

        /**
         * @param a 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger LCM(const UnsignedBigInteger& a, const UnsignedBigInteger& b);

        /**
         * @param min 
         * @param max_excluded 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_number(const UnsignedBigInteger& min, const UnsignedBigInteger& max_excluded);

        /**
         * @param p 
         * @return true 
         * @return false 
         */
        bool is_probably_prime(const UnsignedBigInteger& p);

        /**
         * @param bits 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_big_prime(size_t bits);

    } // namespace NumberTheory
} // namespace Crypto