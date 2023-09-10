/**
 * @file signedbiginteger.h
 * @author Krisna Pranav
 * @brief signed big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "unsignedbiginteger.h"
#include <mods/span.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto 
{

    struct SignedDivisionResult;

    class SignedBigInteger 
    {
    public:

        /**
         * @param x 
         */
        SignedBigInteger(i32 x) : m_sign(x < 0) , m_unsigned_data(abs(x))
        { }

        /**
         * @param unsigned_data 
         * @param sign 
         */
        SignedBigInteger(UnsignedBigInteger&& unsigned_data, bool sign)
            : m_sign(sign)
            , m_unsigned_data(move(unsigned_data))
        { }

        /**
         * @param unsigned_data 
         */
        explicit SignedBigInteger(UnsignedBigInteger unsigned_data)
            : m_sign(false)
            , m_unsigned_data(move(unsigned_data))
        {
        }

        /// @brief Construct a new Signed Big Integer object
        SignedBigInteger()
            : m_sign(false)
            , m_unsigned_data()
        {
        }

        /**
         * @return SignedBigInteger 
         */
        static SignedBigInteger create_invalid()
        {
            return { UnsignedBigInteger::create_invalid(), false };
        }

        /**
         * @param data 
         * @return SignedBigInteger 
         */
        static SignedBigInteger import_data(const Mods::StringView& data) 
        { 
            return import_data((const u8*)data.characters_without_null_termination(), data.length()); 
        }

        /**
         * @param ptr 
         * @param length 
         * @return SignedBigInteger 
         */
        static SignedBigInteger import_data(const u8* ptr, size_t length);

        /**
         * @param remove_leading_zeros 
         * @return size_t 
         */
        size_t export_data(Bytes, bool remove_leading_zeros = false) const;

        /**
         * @param str 
         * @return SignedBigInteger 
         */
        static SignedBigInteger from_base10(StringView str);

        /**
         * @return String 
         */
        String to_base10() const;

        /**
         * @return const UnsignedBigInteger& 
         */
        const UnsignedBigInteger& unsigned_value() const 
        { 
            return m_unsigned_data; 
        }

        /**
         * @return const Vector<u32, STARTING_WORD_SIZE> 
         */
        const Vector<u32, STARTING_WORD_SIZE> words() const 
        { 
            return m_unsigned_data.words(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_negative() const 
        { 
            return m_sign; 
        }

        void negate() 
        { 
            m_sign = !m_sign; 
        }

        void set_to_0() 
        { 
            m_unsigned_data.set_to_0(); 
        }

        /**
         * @param other 
         */
        void set_to(i32 other)
        {
            m_unsigned_data.set_to((u32)other);
            m_sign = other < 0;
        }

        /**
         * @param other 
         */
        void set_to(const SignedBigInteger& other)
        {
            m_unsigned_data.set_to(other.m_unsigned_data);
            m_sign = other.m_sign;
        }

        void invalidate()
        {
            m_unsigned_data.invalidate();
        }

        bool is_invalid() const 
        { 
            return m_unsigned_data.is_invalid(); 
        }

        /**
         * @return size_t 
         */
        size_t length() const 
        { 
            return m_unsigned_data.length() + 1; 
        }

        /**
         * @return size_t 
         */
        size_t trimmed_length() const 
        { 
            return m_unsigned_data.trimmed_length() + 1; 
        };

        /**
         * @param other 
         * @return SignedBigInteger 
         */
        SignedBigInteger plus(const SignedBigInteger& other) const;
        SignedBigInteger minus(const SignedBigInteger& other) const;
        SignedBigInteger bitwise_or(const SignedBigInteger& other) const;
        SignedBigInteger bitwise_and(const SignedBigInteger& other) const;
        SignedBigInteger bitwise_xor(const SignedBigInteger& other) const;

        SignedBigInteger bitwise_not() const;

        /**
         * @param num_bits 
         * @return SignedBigInteger 
         */
        SignedBigInteger shift_left(size_t num_bits) const;

        /**
         * @param other 
         * @return SignedBigInteger 
         */
        SignedBigInteger multiplied_by(const SignedBigInteger& other) const;

        /**
         * @param divisor 
         * @return SignedDivisionResult 
         */
        SignedDivisionResult divided_by(const SignedBigInteger& divisor) const;

        /**
         * @param other 
         * @return SignedBigInteger 
         */
        SignedBigInteger plus(const UnsignedBigInteger& other) const;
        SignedBigInteger minus(const UnsignedBigInteger& other) const;
        SignedBigInteger bitwise_or(const UnsignedBigInteger& other) const;
        SignedBigInteger bitwise_and(const UnsignedBigInteger& other) const;
        SignedBigInteger bitwise_xor(const UnsignedBigInteger& other) const;
        SignedBigInteger multiplied_by(const UnsignedBigInteger& other) const;
        SignedDivisionResult divided_by(const UnsignedBigInteger& divisor) const;

        /**
         * @param bit_index 
         */
        void set_bit_inplace(size_t bit_index);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const SignedBigInteger& other) const;
        bool operator!=(const SignedBigInteger& other) const;
        bool operator<(const SignedBigInteger& other) const;

        bool operator==(const UnsignedBigInteger& other) const;
        bool operator!=(const UnsignedBigInteger& other) const;
        bool operator<(const UnsignedBigInteger& other) const;

    private:
        bool m_sign { false };
        UnsignedBigInteger m_unsigned_data;
    }; // class SignedBigInteger 

    struct SignedDivisionResult 
    {
        Crypto::SignedBigInteger quotient;
        Crypto::SignedBigInteger remainder;
    };

} // namespace Crypto

/**
 * @param stream 
 * @param value 
 * @return const LogStream& 
 */
inline const LogStream&
operator<<(const LogStream& stream, const Crypto::SignedBigInteger value)
{
    if (value.is_invalid()) {
        stream << "Invalid BigInt";
        return stream;
    }

    if (value.is_negative())
        stream << "-";

    stream << value.unsigned_value();
    return stream;
}

/**
 * @param string 
 * @param length 
 * @return Crypto::SignedBigInteger 
 */
inline Crypto::SignedBigInteger
operator""_sbigint(const char* string, size_t length)
{
    return Crypto::SignedBigInteger::from_base10({ string, length });
}
