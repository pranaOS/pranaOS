/**
 * @file modularfunctions.cpp
 * @author Krisna Pranav
 * @brief Modular Functions
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/debug.h>
#include <libcrypto/bigint/algorithms/unsignedbigintegeralgorithms.h>
#include <libcrypto/numbertheory/modularfunctions.h>

namespace Crypto 
{

    namespace NumberTheory 
    {

        /**
         * @param a_ 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger ModularInverse(UnsignedBigInteger const& a_, UnsignedBigInteger const& b)
        {
            if (b == 1)
                return { 1 };

            UnsignedBigInteger temp_1;
            UnsignedBigInteger temp_2;
            UnsignedBigInteger temp_3;
            UnsignedBigInteger temp_4;
            UnsignedBigInteger temp_minus;
            UnsignedBigInteger temp_quotient;
            UnsignedBigInteger temp_d;
            UnsignedBigInteger temp_u;
            UnsignedBigInteger temp_v;
            UnsignedBigInteger temp_x;
            UnsignedBigInteger result;

            UnsignedBigIntegerAlgorithms::modular_inverse_without_allocation(a_, b, temp_1, temp_2, temp_3, temp_4, temp_minus, temp_quotient, temp_d, temp_u, temp_v, temp_x, result);
            return result;
        }

        /**
         * @param b 
         * @param e 
         * @param m 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger ModularPower(UnsignedBigInteger const& b, UnsignedBigInteger const& e, UnsignedBigInteger const& m)
        {
            if (m == 1)
                return 0;

            if (m.is_odd()) {
                UnsignedBigInteger temp_z0 { 0 };
                UnsignedBigInteger temp_rr { 0 };
                UnsignedBigInteger temp_one { 0 };
                UnsignedBigInteger temp_z { 0 };
                UnsignedBigInteger temp_zz { 0 };
                UnsignedBigInteger temp_x { 0 };
                UnsignedBigInteger temp_extra { 0 };

                UnsignedBigInteger result;
                UnsignedBigIntegerAlgorithms::montgomery_modular_power_with_minimal_allocations(b, e, m, temp_z0, temp_rr, temp_one, temp_z, temp_zz, temp_x, temp_extra, result);
                return result;
            }

            UnsignedBigInteger ep { e };
            UnsignedBigInteger base { b };

            UnsignedBigInteger result;
            UnsignedBigInteger temp_1;
            UnsignedBigInteger temp_2;
            UnsignedBigInteger temp_3;
            UnsignedBigInteger temp_4;
            UnsignedBigInteger temp_multiply;
            UnsignedBigInteger temp_quotient;
            UnsignedBigInteger temp_remainder;

            UnsignedBigIntegerAlgorithms::destructive_modular_power_without_allocation(ep, base, m, temp_1, temp_2, temp_3, temp_4, temp_multiply, temp_quotient, temp_remainder, result);

            return result;
        }

        /**
         * @param a 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger GCD(UnsignedBigInteger const& a, UnsignedBigInteger const& b)
        {
            UnsignedBigInteger temp_a { a };
            UnsignedBigInteger temp_b { b };
            UnsignedBigInteger temp_1;
            UnsignedBigInteger temp_2;
            UnsignedBigInteger temp_3;
            UnsignedBigInteger temp_4;
            UnsignedBigInteger temp_quotient;
            UnsignedBigInteger temp_remainder;
            UnsignedBigInteger output;

            UnsignedBigIntegerAlgorithms::destructive_GCD_without_allocation(temp_a, temp_b, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder, output);

            return output;
        }

        /**
         * @param a 
         * @param b 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger LCM(UnsignedBigInteger const& a, UnsignedBigInteger const& b)
        {
            UnsignedBigInteger temp_a { a };
            UnsignedBigInteger temp_b { b };
            UnsignedBigInteger temp_1;
            UnsignedBigInteger temp_2;
            UnsignedBigInteger temp_3;
            UnsignedBigInteger temp_4;
            UnsignedBigInteger temp_quotient;
            UnsignedBigInteger temp_remainder;
            UnsignedBigInteger gcd_output;
            UnsignedBigInteger output { 0 };

            UnsignedBigIntegerAlgorithms::destructive_GCD_without_allocation(temp_a, temp_b, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder, gcd_output);
            if (gcd_output == 0) {
                dbgln_if(NT_DEBUG, "GCD is zero");
                return output;
            }

            UnsignedBigIntegerAlgorithms::divide_without_allocation(a, gcd_output, temp_1, temp_2, temp_3, temp_4, temp_quotient, temp_remainder);
            UnsignedBigIntegerAlgorithms::multiply_without_allocation(temp_quotient, b, temp_1, temp_2, temp_3, output);

            dbgln_if(NT_DEBUG, "quot: {} rem: {} out: {}", temp_quotient, temp_remainder, output);

            return output;
        }

        /**
         * @param n 
         * @param tests 
         * @return true 
         * @return false 
         */
        static bool MR_primality_test(UnsignedBigInteger n, Vector<UnsignedBigInteger, 256> const& tests)
        {
            VERIFY(!(n < 4));
            auto predecessor = n.minus({ 1 });
            auto d = predecessor;
            size_t r = 0;

            {
                auto div_result = d.divided_by(2);
                while (div_result.remainder == 0) {
                    d = div_result.quotient;
                    div_result = d.divided_by(2);
                    ++r;
                }
            }
            if (r == 0) {
                return n == 2;
            }

            for (auto& a : tests) {
                VERIFY(a < n);
                auto x = ModularPower(a, d, n);
                if (x == 1 || x == predecessor)
                    continue;
                bool skip_this_witness = false;
                
                for (size_t i = 0; i < r - 1; ++i) {
                    x = ModularPower(x, 2, n);
                    if (x == predecessor) {
                        skip_this_witness = true;
                        break;
                    }
                }
                if (skip_this_witness)
                    continue;
                return false; 
            }

            return true; 
        }

        /**
         * @param min 
         * @param max_excluded 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_number(UnsignedBigInteger const& min, UnsignedBigInteger const& max_excluded)
        {
            VERIFY(min < max_excluded);
            auto range = max_excluded.minus(min);
            UnsignedBigInteger base;
            auto size = range.trimmed_length() * sizeof(u32) + 2;
            
            auto buffer = ByteBuffer::create_uninitialized(size).release_value_but_fixme_should_propagate_errors(); // FIXME: Handle possible OOM situation.
            auto* buf = buffer.data();

            fill_with_random(buf, size);
            UnsignedBigInteger random { buf, size };

            auto divmod = random.divided_by(range);

            return divmod.remainder.plus(min);
        }

        /**
         * @param p 
         * @return true 
         * @return false 
         */
        bool is_probably_prime(UnsignedBigInteger const& p)
        {
            if (p < 49) {
                u32 p_value = p.words()[0];

                if (p_value == 2 || p_value == 3 || p_value == 5 || p_value == 7)
                    return true;
                
                if (p_value % 2 == 0 || p_value % 3 == 0 || p_value % 5 == 0 || p_value % 7 == 0)
                    return false;
                
                return true;
            }

            Vector<UnsignedBigInteger, 256> tests;
            
            tests.append(UnsignedBigInteger(2));
            tests.append(UnsignedBigInteger(3));
            tests.append(UnsignedBigInteger(5));
            tests.append(UnsignedBigInteger(7));
            tests.append(UnsignedBigInteger(11));
            tests.append(UnsignedBigInteger(13));
            UnsignedBigInteger seventeen { 17 };
            for (size_t i = tests.size(); i < 256; ++i) {
                tests.append(random_number(seventeen, p.minus(2)));
            }

            return MR_primality_test(p, tests);
        }

        /**
         * @param bits 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger random_big_prime(size_t bits)
        {
            VERIFY(bits >= 33);
            UnsignedBigInteger min = UnsignedBigInteger::from_base(10, "6074001000").shift_left(bits - 33);
            UnsignedBigInteger max = UnsignedBigInteger { 1 }.shift_left(bits).minus(1);
            for (;;) {
                auto p = random_number(min, max);
                if ((p.words()[0] & 1) == 0) {
                    continue;
                }
                if (is_probably_prime(p))
                    return p;
            }
        }

    } // namespace NumberTheory

} // namespace Crypto
