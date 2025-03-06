/**
 * @file signedbiginteger.h
 * @author Krisna Pranav
 * @brief signed big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto 
{

    struct SignedDivisionResult;

    class SignedBigInteger 
    {
    public:
        /**
         * @brief Construct a new SignedBigInteger object
         * 
         * @param x 
         */
        SignedBigInteger(i32 x)
            : m_sign(x < 0)
            , m_unsigned_data(abs(x))
        {
        }

        /**
         * @brief Construct a new SignedBigInteger object
         * 
         * @param unsigned_data 
         * @param sign 
         */
        SignedBigInteger(UnsignedBigInteger&& unsigned_data, bool sign)
            : m_sign(sign)
            , m_unsigned_data(move(unsigned_data))
        {
            ensure_sign_is_valid();
        }

        /**
         * @brief Construct a new SignedBigInteger object
         * 
         * @param unsigned_data 
         */
        explicit SignedBigInteger(UnsignedBigInteger unsigned_data)
            : m_sign(false)
            , m_unsigned_data(move(unsigned_data))
        {
        }

        /**
         * @brief Construct a new SignedBigInteger object
         * 
         */
        SignedBigInteger()
            : m_sign(false)
            , m_unsigned_data()
        {
        }

        static SignedBigInteger create_invalid()
        {
            return { UnsignedBigInteger::create_invalid(), false };
        }

        /**
         * @param data 
         * @return SignedBigInteger 
         */
        static SignedBigInteger import_data(StringView data) 
        { 
            return import_data((u8 const*)data.characters_without_null_termination(), data.length()); 
        }

        /**
         * @param ptr 
         * @param length 
         * @return SignedBigInteger 
         */
        static SignedBigInteger import_data(u8 const* ptr, size_t length);

        /**
         * @brief Create a from object
         * 
         * @param value 
         * @return SignedBigInteger 
         */
        static SignedBigInteger create_from(i64 value)
        {
            auto sign = false;
            u64 unsigned_value;
            if (value < 0) {
                unsigned_value = static_cast<u64>(-(value + 1)) + 1;
                sign = true;
            } else {
                unsigned_value = value;
            }
            return SignedBigInteger { UnsignedBigInteger::create_from(unsigned_value), sign };
        }

        /**
         * @param remove_leading_zeros 
         * @return size_t 
         */
        size_t export_data(Bytes, bool remove_leading_zeros = false) const;

        /**
         * @param N 
         * @param str 
         * @return SignedBigInteger 
         */
        static SignedBigInteger from_base(u16 N, StringView str);

        /**
         * @param N 
         * @return String 
         */
        String to_base(u16 N) const;

        u64 to_u64() const;
        double to_double() const;

        /**
         * @return UnsignedBigInteger const& 
         */
        UnsignedBigInteger const& unsigned_value() const 
        { 
            return m_unsigned_data; 
        }

        Vector<u32, STARTING_WORD_SIZE> const words() const 
        { 
            return m_unsigned_data.words(); 
        }

        bool is_negative() const 
        { 
            return m_sign; 
        }

        void negate()
        {
            if (!m_unsigned_data.is_zero())
                m_sign = !m_sign;
        }

        void set_to_0()
        {
            m_unsigned_data.set_to_0();
            m_sign = false;
        }

        /**
         * @brief Set the to object
         * 
         * @param other 
         */
        void set_to(i32 other)
        {
            m_unsigned_data.set_to((u32)other);
            m_sign = other < 0;
        }

        /**
         * @brief Set the to object
         * 
         * @param other 
         */
        void set_to(SignedBigInteger const& other)
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

        size_t length() const 
        { 
            return m_unsigned_data.length() + 1; 
        }
        
        size_t trimmed_length() const 
        { 
            return m_unsigned_data.trimmed_length() + 1; 
        };

        /**
         * @param other 
         * @return SignedBigInteger 
         */
        SignedBigInteger plus(SignedBigInteger const& other) const;
        SignedBigInteger minus(SignedBigInteger const& other) const;
        SignedBigInteger bitwise_or(SignedBigInteger const& other) const;
        SignedBigInteger bitwise_and(SignedBigInteger const& other) const;
        SignedBigInteger bitwise_xor(SignedBigInteger const& other) const;

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
        SignedBigInteger multiplied_by(SignedBigInteger const& other) const;

        /**
         * @param divisor 
         * @return SignedDivisionResult 
         */
        SignedDivisionResult divided_by(SignedBigInteger const& divisor) const;

        /**
         * @param other 
         * @return SignedBigInteger 
         */
        SignedBigInteger plus(UnsignedBigInteger const& other) const;
        SignedBigInteger minus(UnsignedBigInteger const& other) const;
        SignedBigInteger multiplied_by(UnsignedBigInteger const& other) const;

        /**
         * @param divisor 
         * @return SignedDivisionResult 
         */
        SignedDivisionResult divided_by(UnsignedBigInteger const& divisor) const;

        /**
         * @return u32 
         */
        u32 hash() const;

        /**
         * @brief Set the bit inplace object
         * 
         * @param bit_index 
         */
        void set_bit_inplace(size_t bit_index);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(SignedBigInteger const& other) const;
        bool operator!=(SignedBigInteger const& other) const;
        bool operator<(SignedBigInteger const& other) const;
        bool operator<=(SignedBigInteger const& other) const;
        bool operator>(SignedBigInteger const& other) const;
        bool operator>=(SignedBigInteger const& other) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(UnsignedBigInteger const& other) const;
        bool operator!=(UnsignedBigInteger const& other) const;
        bool operator<(UnsignedBigInteger const& other) const;
        bool operator>(UnsignedBigInteger const& other) const;

    private:
        void ensure_sign_is_valid()
        {
            if (m_sign && m_unsigned_data.is_zero())
                m_sign = false;
        }

        bool m_sign { false };
        UnsignedBigInteger m_unsigned_data;
    }; // class SignedBigInteger 

    struct SignedDivisionResult {
        Crypto::SignedBigInteger quotient;
        Crypto::SignedBigInteger remainder;
    }; // struct SignedDivisionResult

} // namespace Crypto

template<>
struct Mods::Formatter<Crypto::SignedBigInteger> : Mods::Formatter<Crypto::UnsignedBigInteger> {
    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder&, Crypto::SignedBigInteger const&);
};

/**
 * @param string 
 * @param length 
 * @return Crypto::SignedBigInteger 
 */
inline Crypto::SignedBigInteger
operator""_sbigint(char const* string, size_t length)
{
    return Crypto::SignedBigInteger::from_base(10, { string, length });
}
