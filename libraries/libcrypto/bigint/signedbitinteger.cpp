/**
 * @file signedbitinteger.cpp
 * @author Krisna Pranav
 * @brief signed bit integer
 * @version 6.0
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_builder.h>
#include "signedbiginteger.h"


namespace Crypto 
{

    /**
     * @param ptr 
     * @param length 
     * @return SignedBigInteger 
     */
    SignedBigInteger SignedBigInteger::import_data(const u8* ptr, size_t length)
    {
        bool sign = *ptr;
        auto unsigned_data = UnsignedBigInteger::import_data(ptr + 1, length - 1);

        return { move(unsigned_data), sign };
    }

    /**
     * @param data 
     * @param remove_leading_zeros 
     * @return size_t 
     */
    size_t SignedBigInteger::export_data(Bytes data, bool remove_leading_zeros) const
    {
        ASSERT(!remove_leading_zeros);

        data[0] = m_sign;
        auto bytes_view = data.slice(1, data.size() - 1);
        
        return m_unsigned_data.export_data(bytes_view, remove_leading_zeros) + 1;
    }

    /**
     * @param str 
     * @return SignedBigInteger 
     */
    SignedBigInteger SignedBigInteger::from_base10(StringView str)
    {
        bool sign = false;

        if (str.length() > 1) {
            auto maybe_sign = str[0];
            if (maybe_sign == '-') {
                str = str.substring_view(1, str.length() - 1);
                sign = true;
            }
            if (maybe_sign == '+')
                str = str.substring_view(1, str.length() - 1);
        }
        
        auto unsigned_data = UnsignedBigInteger::from_base10(str);

        return { move(unsigned_data), sign };
    }

    /**
     * @return String 
     */
    String SignedBigInteger::to_base10() const
    {
        StringBuilder builder;

        if (m_sign)
            builder.append('-');

        builder.append(m_unsigned_data.to_base10());

        return builder.to_string();
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::plus(const SignedBigInteger& other) const
    {
        if (m_sign == other.m_sign)
            return { other.m_unsigned_data.plus(m_unsigned_data), m_sign };

        return m_sign ? other.minus(this->m_unsigned_data) : minus(other.m_unsigned_data);
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::minus(const SignedBigInteger& other) const
    {
        if (m_sign != other.m_sign) {
            SignedBigInteger result { other.m_unsigned_data.plus(this->m_unsigned_data) };
            if (m_sign)
                result.negate();
            return result;
        }

        if (!m_sign) {
            if (m_unsigned_data < other.m_unsigned_data) {
                return { other.m_unsigned_data.minus(m_unsigned_data), true };
            }

            return SignedBigInteger { m_unsigned_data.minus(other.m_unsigned_data) };
        }

        if (m_unsigned_data < other.m_unsigned_data) {
            return SignedBigInteger { m_unsigned_data.minus(other.m_unsigned_data) };
        }

        return { other.m_unsigned_data.minus(m_unsigned_data), true };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::plus(const UnsignedBigInteger& other) const
    {
        if (m_sign) {
            if (other < m_unsigned_data)
                return { m_unsigned_data.minus(other), true };

            return { other.minus(m_unsigned_data), false };
        }

        return { m_unsigned_data.plus(other), false };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::minus(const UnsignedBigInteger& other) const
    {
        if (m_sign)
            return { m_unsigned_data.plus(m_unsigned_data), true };

        if (other < m_unsigned_data)
            return { m_unsigned_data.minus(other), false };

        return { other.minus(m_unsigned_data), true };
    }

    /** 
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_or(const UnsignedBigInteger& other) const
    {
        return { unsigned_value().bitwise_or(other), m_sign };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_and(const UnsignedBigInteger& other) const
    {
        return { unsigned_value().bitwise_and(other), false };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_xor(const UnsignedBigInteger& other) const
    {
        return { unsigned_value().bitwise_xor(other), m_sign };
    }
    
    /**
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_not() const
    {
        return { unsigned_value().bitwise_not(), !m_sign };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_or(const SignedBigInteger& other) const
    {
        auto result = bitwise_or(other.unsigned_value());

        if (other.is_negative())
            result.negate();

        return result;
    }
    
    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_and(const SignedBigInteger& other) const
    {
        auto result = bitwise_and(other.unsigned_value());

        result.m_sign = is_negative() || other.is_negative();

        return result;
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::bitwise_xor(const SignedBigInteger& other) const
    {
        auto result = bitwise_xor(other.unsigned_value());

        result.m_sign = is_negative() ^ other.is_negative();

        return result;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator==(const UnsignedBigInteger& other) const
    {
        if (m_sign)
            return false;

        return m_unsigned_data == other;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator!=(const UnsignedBigInteger& other) const
    {
        if (m_sign)
            return true;

        return m_unsigned_data != other;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator<(const UnsignedBigInteger& other) const
    {
        if (m_sign)
            return true;

        return m_unsigned_data < other;
    }

    /**
     * @param num_bits 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::shift_left(size_t num_bits) const
    {
        return SignedBigInteger { m_unsigned_data.shift_left(num_bits), m_sign };
    }

    /**
     * @param other 
     * @return FLATTEN 
     */
    FLATTEN SignedBigInteger SignedBigInteger::multiplied_by(const SignedBigInteger& other) const
    {
        bool result_sign = m_sign ^ other.m_sign;
        return { m_unsigned_data.multiplied_by(other.m_unsigned_data), result_sign };
    }

    /**
     * @param divisor 
     * @return FLATTEN 
     */
    FLATTEN SignedDivisionResult SignedBigInteger::divided_by(const SignedBigInteger& divisor) const
    {
        bool result_sign = m_sign ^ divisor.m_sign;
        auto unsigned_division_result = m_unsigned_data.divided_by(divisor.m_unsigned_data);

        return {
            { move(unsigned_division_result.quotient), result_sign },
            { move(unsigned_division_result.remainder), m_sign }
        };
    }

    /**
     * @param bit_index 
     */
    void SignedBigInteger::set_bit_inplace(size_t bit_index)
    {
        m_unsigned_data.set_bit_inplace(bit_index);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator==(const SignedBigInteger& other) const
    {
        if (is_invalid() != other.is_invalid())
            return false;

        if (m_unsigned_data == 0 && other.m_unsigned_data == 0)
            return true;

        return m_sign == other.m_sign && m_unsigned_data == other.m_unsigned_data;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator!=(const SignedBigInteger& other) const
    {
        return !(*this == other);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool SignedBigInteger::operator<(const SignedBigInteger& other) const
    {
        if (m_sign ^ other.m_sign)
            return m_sign;

        if (m_sign)
            return other.m_unsigned_data < m_unsigned_data;

        return m_unsigned_data < other.m_unsigned_data;
    }

}
