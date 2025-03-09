/**
 * @file modularfunctions.h
 * @author Krisna Pranav
 * @brief Modular Functions
 * @version 6.0
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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
        UnsignedBigInteger ModularInverse(UnsignedBigInteger const& a_, UnsignedBigInteger const& b);

        /**
         * @param b 
         * @param e 
         * @param m 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger ModularPower(UnsignedBigInteger const& b, UnsignedBigInteger const& e, UnsignedBigInteger const& m);

        /**
         * @tparam IntegerType 
         * @param b 
         * @param e 
         * @return IntegerType 
         */
        template<typename IntegerType>
        static IntegerType Power(IntegerType const& b, IntegerType const& e)
        {
            IntegerType ep { e };
            IntegerType base { b };
            IntegerType exp { 1 };

            while (!(ep < IntegerType { 1 })) {
                if (ep.words()[0] % 2 == 1)
                    exp.set_to(exp.multiplied_by(base));

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
        UnsignedBigInteger GCD(UnsignedBigInteger const& a, UnsignedBigInteger const& b);

        /**
         * @param a 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger LCM(UnsignedBigInteger const& a, UnsignedBigInteger const& b);

        /**
         * @param min 
         * @param max_excluded 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_number(UnsignedBigInteger const& min, UnsignedBigInteger const& max_excluded);

        /**
         * @param p 
         * @return true 
         * @return false 
         */
        bool is_probably_prime(UnsignedBigInteger const& p);

        /**
         * @param bits 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_big_prime(size_t bits);

    } // namespace NumberTheory

} // namespace Crypto
