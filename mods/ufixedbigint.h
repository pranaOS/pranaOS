/**
 * @file ufixedbigint.h
 * @author Krisna Pranav
 * @brief fixed big int
 * @version 6.0
 * @date 2025-02-16
 *
 * @copyright Copyright (c) 2021-2025, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/builtinwrappers.h>
#include <mods/checked.h>
#include <mods/concept.h>
#include <mods/format.h>
#include <mods/numericlimits.h>
#include <mods/stdlibextradetail.h>
#include <mods/stdlibextra.h>
#include <mods/stringbuilder.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
        requires(sizeof(T) >= sizeof(u64) && IsUnsigned<T>)
    class UFixedBigInt;

    /**
     * @tparam T 
     */
    template <typename T>
    constexpr inline bool Detail::IsUnsigned<UFixedBigInt<T>> = true;

    /**
     * @tparam T 
     */
    template <typename T>
    constexpr inline bool Detail::IsSigned<UFixedBigInt<T>> = false;

    /**
     * @tparam T 
     */
    template <typename T>
    struct NumericLimits<UFixedBigInt<T>>
    {
        /**
         * @return constexpr UFixedBigInt<T> 
         */
        static constexpr UFixedBigInt<T> min()
        {
            return 0;
        }

        /**
         * @return constexpr UFixedBigInt<T> 
         */
        static constexpr UFixedBigInt<T> max()
        {
            return {NumericLimits<T>::max(), NumericLimits<T>::max()};
        }

        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_signed()
        {
            return false;
        }
    }; // struct NumericLimits<UFixedBigInt<T>>

    /**
     * @tparam T 
     */
    template <Unsigned T>
    struct UFixedBigIntMultiplicationResult
    {
        T low;
        T high;
    }; // struct UFixedBigIntMultiplicationResult

    /**
     * @tparam T 
     */
    template <typename T>
        requires(sizeof(T) >= sizeof(u64) && IsUnsigned<T>)
    class UFixedBigInt
    {
    public:
        using R = UFixedBigInt<T>;

        constexpr UFixedBigInt() = default;
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr UFixedBigInt(U low)
            : m_low(low), m_high(0u)
        {
        }
        template <Unsigned U, Unsigned U2>
            requires(sizeof(T) >= sizeof(U) && sizeof(T) >= sizeof(U2))
        constexpr UFixedBigInt(U low, U2 high)
            : m_low(low), m_high(high)
        {
        }

        /**
         * @return constexpr T& 
         */
        constexpr T& low()
        {
            return m_low;
        }
        constexpr const T& low() const
        {
            return m_low;
        }
        constexpr T& high()
        {
            return m_high;
        }
        constexpr const T& high() const
        {
            return m_high;
        }

        /**
         * @return Span<u8> 
         */
        Span<u8> bytes()
        {
            return Span<u8>(reinterpret_cast<u8*>(this), sizeof(R));
        }
        Span<const u8> bytes() const
        {
            return Span<const u8>(reinterpret_cast<u8 const*>(this), sizeof(R));
        }

        /**
         * @tparam U 
         * @return U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        explicit operator U() const
        {
            return static_cast<U>(m_low);
        }

        constexpr size_t clz() const
            requires(IsSame<T, u64>)
        {
            if(m_high)
                return count_leading_zeroes(m_high);
            else
                return sizeof(T) * 8 + count_leading_zeroes(m_low);
        }

        constexpr size_t clz() const
            requires(!IsSame<T, u64>)
        {
            if(m_high)
                return m_high.clz();
            else
                return sizeof(T) * 8 + m_low.clz();
        }

        constexpr size_t ctz() const
            requires(IsSame<T, u64>)
        {
            if(m_low)
                return count_trailing_zeroes(m_low);
            else
                return sizeof(T) * 8 + count_trailing_zeroes(m_high);
        }

        constexpr size_t ctz() const
            requires(!IsSame<T, u64>)
        {
            if(m_low)
                return m_low.ctz();
            else
                return sizeof(T) * 8 + m_high.ctz();
        }

        constexpr size_t popcnt() const
            requires(IsSame<T, u64>)
        {
            return __builtin_popcntll(m_low) + __builtin_popcntll(m_high);
        }

        constexpr size_t popcnt() const
            requires(!IsSame<T, u64>)
        {
            return m_low.popcnt() + m_high.popcnt();
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr bool operator!() const
        {
            return !m_low && !m_high;
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr explicit operator bool() const
        {
            return m_low || m_high;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator==(const U& other) const
        {
            return !m_high && m_low == other;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator!=(const U& other) const
        {
            return m_high || m_low != other;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator>(const U& other) const
        {
            return m_high || m_low > other;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator<(const U& other) const
        {
            return !m_high && m_low < other;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator>=(const U& other) const
        {
            return *this == other || *this > other;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr bool operator<=(const U& other) const
        {
            return *this == other || *this < other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(const R& other) const
        {
            return m_low == other.low() && m_high == other.high();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(const R& other) const
        {
            return m_low != other.low() || m_high != other.high();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>(const R& other) const
        {
            return m_high > other.high() || (m_high == other.high() && m_low > other.low());
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<(const R& other) const
        {
            return m_high < other.high() || (m_high == other.high() && m_low < other.low());
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>=(const R& other) const
        {
            return *this == other || *this > other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<=(const R& other) const
        {
            return *this == other || *this < other;
        }

        /**
         * @return constexpr R 
         */
        constexpr R operator~() const
        {
            return {~m_low, ~m_high};
        }
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr U operator&(const U& other) const
        {
            return static_cast<const U>(m_low) & other;
        }
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R operator|(const U& other) const
        {
            return {m_low | other, m_high};
        }
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R operator^(const U& other) const
        {
            return {m_low ^ other, m_high};
        }

        /**
         * @tparam U 
         * @param shift 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator<<(const U& shift) const
        {
            if(shift >= sizeof(R) * 8u)
                return 0u;
            if(shift >= sizeof(T) * 8u)
                return R{0u, m_low << (shift - sizeof(T) * 8u)};
            if(!shift)
                return *this;

            T overflow = m_low >> (sizeof(T) * 8u - shift);
            return R{m_low << shift, (m_high << shift) | overflow};
        }

        /**
         * @tparam U 
         * @param shift 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator>>(const U& shift) const
        {
            if(shift >= sizeof(R) * 8u)
                return 0u;
            if(shift >= sizeof(T) * 8u)
                return m_high >> (shift - sizeof(T) * 8u);
            if(!shift)
                return *this;

            T underflow = m_high << (sizeof(T) * 8u - shift);
            return R{(m_low >> shift) | underflow, m_high >> shift};
        }

        /**
         * @tparam U 
         * @param shift 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R rol(const U& shift) const
        {
            return (*this >> sizeof(T) * 8u - shift) | (*this << shift);
        }

        /**
         * @tparam U 
         * @param shift 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R ror(const U& shift) const
        {
            return (*this << sizeof(T) * 8u - shift) | (*this >> shift);
        }

        /**
         * @param other 
         * @return constexpr R 
         */
        constexpr R operator&(const R& other) const
        {
            return {m_low & other.low(), m_high & other.high()};
        }

        /**
         * @param other 
         * @return constexpr R 
         */
        constexpr R operator|(const R& other) const
        {
            return {m_low | other.low(), m_high | other.high()};
        }

        /**
         * @param other 
         * @return constexpr R 
         */
        constexpr R operator^(const R& other) const
        {
            return {m_low ^ other.low(), m_high ^ other.high()};
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R& operator&=(const U& other)
        {
            m_high = 0u;
            m_low &= other;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R& operator|=(const U& other)
        {
            m_low |= other;
            return *this;
        }
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R& operator^=(const U& other)
        {
            m_low ^= other;
            return *this;
        }
        template <Unsigned U>
        constexpr R& operator>>=(const U& other)
        {
            *this = *this >> other;
            return *this;
        }
        template <Unsigned U>
        constexpr R& operator<<=(const U& other)
        {
            *this = *this << other;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr R& 
         */
        constexpr R& operator&=(const R& other)
        {
            m_high &= other.high();
            m_low &= other.low();
            return *this;
        }

        /**
         * @param other 
         * @return constexpr R& 
         */
        constexpr R& operator|=(const R& other)
        {
            m_high |= other.high();
            m_low |= other.low();
            return *this;
        }

        /**
         * @param other 
         * @return constexpr R& 
         */
        constexpr R& operator^=(const R& other)
        {
            m_high ^= other.high();
            m_low ^= other.low();
            return *this;
        }

        /**
         * @return constexpr size_t 
         */
        static constexpr size_t my_size()
        {
            return sizeof(R);
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U) && IsSame<T, u64>)
        constexpr R addc(const U other, bool& carry) const
        {
            bool low_carry = Checked<T>::addition_would_overflow(m_low, other);
            low_carry |= Checked<T>::addition_would_overflow(m_low, carry);
            bool high_carry = Checked<T>::addition_would_overflow(m_high, low_carry);

            T lower = m_low + other + carry;
            T higher = m_high + low_carry;

            carry = high_carry;

            return {
                lower,
                higher};
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(my_size() > sizeof(U) && sizeof(T) > sizeof(u64))
        constexpr R addc(const U& other, bool& carry) const
        {
            T lower = m_low.addc(other, carry);
            T higher = m_high.addc(0u, carry);

            return {
                lower,
                higher};
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(IsSame<R, U> && IsSame<T, u64>)
        constexpr R addc(const U& other, bool& carry) const
        {
            bool low_carry = Checked<T>::addition_would_overflow(m_low, other.low());
            bool high_carry = Checked<T>::addition_would_overflow(m_high, other.high());

            T lower = m_low + other.low();
            T higher = m_high + other.high();
            low_carry |= Checked<T>::addition_would_overflow(lower, carry);
            high_carry |= Checked<T>::addition_would_overflow(higher, low_carry);

            lower += carry;
            higher += low_carry;
            carry = high_carry;

            return {
                lower,
                higher};
        }
        template <Unsigned U>
            requires(IsSame<R, U> && sizeof(T) > sizeof(u64))
        constexpr R addc(const U& other, bool& carry) const
        {
            T lower = m_low.addc(other.low(), carry);
            T higher = m_high.addc(other.high(), carry);

            return {
                lower,
                higher};
        }
        template <Unsigned U>
            requires(my_size() < sizeof(U))
        constexpr U addc(const U& other, bool& carry) const
        {
            return other.addc(*this, carry);
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(T) >= sizeof(U))
        constexpr R subc(const U& other, bool& carry) const
        {
            bool low_carry = (!m_low && carry) || (m_low - carry) < other;
            bool high_carry = !m_high && low_carry;

            T lower = m_low - other - carry;
            T higher = m_high - low_carry;
            carry = high_carry;

            return {lower, higher};
        }

        /**
         * @param other 
         * @param carry 
         * @return constexpr R 
         */
        constexpr R subc(const R& other, bool& carry) const
        {
            bool low_carry = (!m_low && carry) || (m_low - carry) < other.low();
            bool high_carry = (!m_high && low_carry) || (m_high - low_carry) < other.high();

            T lower = m_low - other.low() - carry;
            T higher = m_high - other.high() - low_carry;
            carry = high_carry;

            return {lower, higher};
        }

        /**
         * @param other 
         * @return constexpr R 
         */
        constexpr R operator+(bool const& other) const
        {
            bool carry = false; 
            return addc((u8)other, carry);
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator+(const U& other) const
        {
            bool carry = false; 
            return addc(other, carry);
        }

        /**
         * @param other 
         * @return constexpr R 
         */
        constexpr R operator-(bool const& other) const
        {
            bool carry = false; 
            return subc((u8)other, carry);
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator-(const U& other) const
        {
            bool carry = false; 
            return subc(other, carry);
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R& 
         */
        template <Unsigned U>
        constexpr R& operator+=(const U& other)
        {
            *this = *this + other;
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R& 
         */
        template <Unsigned U>
        constexpr R& operator-=(const U& other)
        {
            *this = *this - other;
            return *this;
        }

        /**
         * @return constexpr R 
         */
        constexpr R operator++()
        {
            auto old = *this;
            *this += 1;
            return old;
        }

        /**
         * @return constexpr R& 
         */
        constexpr R& operator++(int)
        {
            *this += 1;
            return *this;
        }

        /**
         * @return constexpr R 
         */
        constexpr R operator--()
        {
            auto old = *this;
            *this -= 1;
            return old;
        }

        /**
         * @return constexpr R& 
         */
        constexpr R& operator--(int)
        {
            *this -= 1;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(my_size() >= sizeof(U))
        constexpr R div_mod(const U& divisor, U& remainder) const
        {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdiv-by-zero"
            if(!divisor)
            {
                int volatile x = 1;
                int volatile y = 0;
                [[maybe_unused]] int volatile z = x / y;
            }
    #pragma GCC diagnostic pop

            if(*this < divisor)
            {
                remainder = static_cast<U>(*this);
                return 0u;
            }
            if(*this == divisor)
            {
                remainder = 0u;
                return 1u;
            }
            if(divisor == 1u)
            {
                remainder = 0u;
                return *this;
            }

            remainder = 0u;
            R quotient = 0u;

            for(ssize_t i = sizeof(R) * 8 - clz() - 1; i >= 0; --i)
            {
                remainder <<= 1u;
                remainder |= (*this >> (size_t)i) & 1u;
                if(remainder >= divisor)
                {
                    remainder -= divisor;
                    quotient |= R{1u} << (size_t)i;
                }
            }

            return quotient;
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator*(U other) const
        {
            R res = 0u;
            R that = *this;
            for(; other != 0u; other >>= 1u)
            {
                if(other & 1u)
                    res += that;
                that <<= 1u;
            }
            return res;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(IsSame<R, U> && IsSame<T, u64>)
        constexpr UFixedBigIntMultiplicationResult<R> wide_multiply(U const& other) const
        {
            auto mult_64_to_128 = [](u64 a, u64 b) -> UFixedBigIntMultiplicationResult<u64>
            {
    #ifdef __SIZEOF_INT128__
                unsigned __int128 result = (unsigned __int128)a * b;
                u64 low = result;
                u64 high = result >> 64;
                return {low, high};
    #else
                u32 a_low = a;
                u32 a_high = (a >> 32);
                u32 b_low = b;
                u32 b_high = (b >> 32);

                u64 ll_result = (u64)a_low * b_low;
                u64 lh_result = (u64)a_low * b_high;
                u64 hl_result = (u64)a_high * b_low;
                u64 hh_result = (u64)a_high * b_high;

                UFixedBigInt<u64> ll{ll_result, 0u};
                UFixedBigInt<u64> lh{lh_result << 32, lh_result >> 32};
                UFixedBigInt<u64> hl{hl_result << 32, hl_result >> 32};
                UFixedBigInt<u64> hh{0u, hh_result};

                UFixedBigInt<u64> result = ll + lh + hl + hh;
                return {result.low(), result.high()};
    #endif
            };

            auto ll_result = mult_64_to_128(m_low, other.low());
            auto lh_result = mult_64_to_128(m_low, other.high());
            auto hl_result = mult_64_to_128(m_high, other.low());
            auto hh_result = mult_64_to_128(m_high, other.high());

            UFixedBigInt<R> ll{R{ll_result.low, ll_result.high}, R{0u, 0u}};
            UFixedBigInt<R> lh{R{0u, lh_result.low}, R{lh_result.high, 0u}};
            UFixedBigInt<R> hl{R{0u, hl_result.low}, R{hl_result.high, 0u}};
            UFixedBigInt<R> hh{R{0u, 0u}, R{hh_result.low, hh_result.high}};

            UFixedBigInt<R> result = ll + lh + hl + hh;
            return {result.low(), result.high()};
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(IsSame<R, U> && sizeof(T) > sizeof(u64))
        constexpr UFixedBigIntMultiplicationResult<R> wide_multiply(U const& other) const
        {
            T left_low = m_low;
            T left_high = m_high;
            T right_low = other.low();
            T right_high = other.high();

            auto ll_result = left_low.wide_multiply(right_low);
            auto lh_result = left_low.wide_multiply(right_high);
            auto hl_result = left_high.wide_multiply(right_low);
            auto hh_result = left_high.wide_multiply(right_high);

            UFixedBigInt<R> ll{R{ll_result.low, ll_result.high}, R{0u, 0u}};
            UFixedBigInt<R> lh{R{0u, lh_result.low}, R{lh_result.high, 0u}};
            UFixedBigInt<R> hl{R{0u, hl_result.low}, R{hl_result.high, 0u}};
            UFixedBigInt<R> hh{R{0u, 0u}, R{hh_result.low, hh_result.high}};

            UFixedBigInt<R> result = ll + lh + hl + hh;
            return {result.low(), result.high()};
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R 
         */
        template <Unsigned U>
        constexpr R operator/(const U& other) const
        {
            U mod{0u}; 
            return div_mod(other, mod);
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr U 
         */
        template <Unsigned U>
        constexpr U operator%(const U& other) const
        {
            R res{0u};
            div_mod(other, res);
            return res;
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R& 
         */
        template <Unsigned U>
        constexpr R& operator*=(const U& other)
        {
            *this = *this * other;
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R& 
         */
        template <Unsigned U>
        constexpr R& operator/=(const U& other)
        {
            *this = *this / other;
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return constexpr R& 
         */
        template <Unsigned U>
        constexpr R& operator%=(const U& other)
        {
            *this = *this % other;
            return *this;
        }

        /**
         * @return constexpr R 
         */
        constexpr R sqrt() const
        {
            if(*this == 1u)
                return 1u;

            ssize_t shift = (sizeof(R) * 8 - clz()) & ~1ULL;

            R res = 0u;
            while(shift >= 0)
            {
                res = res << 1u;
                R large_cand = (res | 1u);
                if(*this >> (size_t)shift >= large_cand * large_cand)
                    res = large_cand;
                shift -= 2;
            }
            return res;
        }

        /**
         * @param exp 
         * @return constexpr R 
         */
        constexpr R pow(u64 exp)
        {
            R x1 = *this;
            R x2 = *this * *this;
            u64 exp_copy = exp;
            for(ssize_t i = sizeof(u64) * 8 - count_leading_zeroes(exp) - 2; i >= 0; --i)
            {
                if(exp_copy & 1u)
                {
                    x2 *= x1;
                    x1 *= x1;
                }
                else
                {
                    x1 *= x2;
                    x2 *= x2;
                }
                exp_copy >>= 1u;
            }
            return x1;
        }

        /**
         * @tparam U 
         */
        template <Unsigned U>
            requires(sizeof(U) > sizeof(u64))
        constexpr R pow(U exp)
        {
            R x1 = *this;
            R x2 = *this * *this;
            U exp_copy = exp;
            for(ssize_t i = sizeof(U) * 8 - exp().clz() - 2; i >= 0; --i)
            {
                if(exp_copy & 1u)
                {
                    x2 *= x1;
                    x1 *= x1;
                }
                else
                {
                    x1 *= x2;
                    x2 *= x2;
                }
                exp_copy >>= 1u;
            }
            return x1;
        }

        /**
         * @tparam U 
         * @param exp 
         * @param mod 
         * @return constexpr U 
         */
        template <Unsigned U>
        constexpr U pow_mod(u64 exp, U mod)
        {
            if(!mod)
                return 0u;

            U res = 1;
            u64 exp_copy = exp;
            for(size_t i = sizeof(u64) - count_leading_zeroes(exp) - 1u; i < exp; ++i)
            {
                res *= res;
                res %= mod;
                if(exp_copy & 1u)
                {
                    res = (*this * res) % mod;
                }
                exp_copy >>= 1u;
            }
            return res;
        }
        template <Unsigned ExpT, Unsigned U>
            requires(sizeof(ExpT) > sizeof(u64))
        constexpr U pow_mod(ExpT exp, U mod)
        {
            if(!mod)
                return 0u;

            U res = 1;
            ExpT exp_copy = exp;
            for(size_t i = sizeof(ExpT) - exp.clz() - 1u; i < exp; ++i)
            {
                res *= res;
                res %= mod;
                if(exp_copy & 1u)
                {
                    res = (*this * res) % mod;
                }
                exp_copy >>= 1u;
            }
            return res;
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t log2()
        {
            return sizeof(R) - clz();
        }

        /**
         * @param base 
         * @return constexpr size_t 
         */
        constexpr size_t logn(u64 base)
        {
            return log2() / (sizeof(u64) - count_leading_zeroes(base));
        }
        template <Unsigned U>
            requires(sizeof(U) > sizeof(u64))
        constexpr size_t logn(U base)
        {
            return log2() / base.log2();
        }

        constexpr u64 fold_or() const
            requires(IsSame<T, u64>)
        {
            return m_low | m_high;
        }

        constexpr u64 fold_or() const
            requires(!IsSame<T, u64>)
        {
            return m_low.fold_or() | m_high.fold_or();
        }
        constexpr bool is_zero_constant_time() const
        {
            return fold_or() == 0;
        }

        constexpr u64 fold_xor_pair(R& other) const
            requires(IsSame<T, u64>)
        {
            return (m_low ^ other.low()) | (m_high ^ other.high());
        }
        constexpr u64 fold_xor_pair(R& other) const
            requires(!IsSame<T, u64>)
        {
            return (m_low.fold_xor_pair(other.low())) | (m_high.fold_xor_pair(other.high()));
        }
        constexpr bool is_equal_to_constant_time(R& other)
        {
            return fold_xor_pair(other) == 0;
        }

    private:
        T m_low;
        T m_high;
    }; // class UFixedBigInt

    /**
     * @tparam U 
     * @tparam T 
     */
    template <Unsigned U, Unsigned T>
        requires(sizeof(U) < sizeof(T) * 2)
    constexpr bool operator<(const U a, UFixedBigInt<T> const& b)
    {
        return b >= a;
    }
    template <Unsigned U, Unsigned T>
        requires(sizeof(U) < sizeof(T) * 2)
    constexpr bool operator>(const U a, UFixedBigInt<T> const& b)
    {
        return b <= a;
    }
    template <Unsigned U, Unsigned T>
        requires(sizeof(U) < sizeof(T) * 2)
    constexpr bool operator<=(const U a, UFixedBigInt<T> const& b)
    {
        return b > a;
    }
    template <Unsigned U, Unsigned T>
        requires(sizeof(U) < sizeof(T) * 2)
    constexpr bool operator>=(const U a, UFixedBigInt<T> const& b)
    {
        return b < a;
    }

    /**
     * @tparam T 
     */
    template <Unsigned T>
    struct Formatter<UFixedBigInt<T>> : StandardFormatter
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(formatter)
        {
        }

        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, UFixedBigInt<T> value)
        {
            if(m_precision.has_value())
                VERIFY_NOT_REACHED();

            if(m_mode == Mode::Pointer)
            {
                VERIFY_NOT_REACHED();
            }
            if(m_mode == Mode::Default)
                m_mode = Mode::Hexadecimal;

            if(!value.high())
            {
                Formatter<T> formatter{*this};
                return formatter.format(builder, value.low());
            }

            u8 base = 0;
            if(m_mode == Mode::Binary)
            {
                base = 2;
            }
            else if(m_mode == Mode::BinaryUppercase)
            {
                base = 2;
            }
            else if(m_mode == Mode::Octal)
            {
                TODO();
            }
            else if(m_mode == Mode::Decimal)
            {
                TODO();
            }
            else if(m_mode == Mode::Hexadecimal)
            {
                base = 16;
            }
            else if(m_mode == Mode::HexadecimalUppercase)
            {
                base = 16;
            }
            else
            {
                VERIFY_NOT_REACHED();
            }
            ssize_t width = m_width.value_or(0);
            ssize_t lower_length = ceil_div(sizeof(T) * 8, (ssize_t)base);
            Formatter<T> formatter{*this};
            formatter.m_width = max(width - lower_length, (ssize_t)0);
            TRY(formatter.format(builder, value.high()));
            TRY(builder.put_literal("'"sv));
            formatter.m_zero_pad = true;
            formatter.m_alternative_form = false;
            formatter.m_width = lower_length;
            TRY(formatter.format(builder, value.low()));
            return {};
        }
    };
} // namespace Mods

using u128 = Mods::UFixedBigInt<u64>;
using u256 = Mods::UFixedBigInt<u128>;
using u512 = Mods::UFixedBigInt<u256>;
using u1024 = Mods::UFixedBigInt<u512>;
using u2048 = Mods::UFixedBigInt<u1024>;
using u4096 = Mods::UFixedBigInt<u2048>;
