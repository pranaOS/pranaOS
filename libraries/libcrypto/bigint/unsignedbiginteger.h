/**
 * @file unsignedbiginteger.h
 * @author Krisna Pranav
 * @brief unsigned big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/logstream.h>
#include <mods/span.h>
#include <mods/types.h>
#include <mods/string.h>
#include <mods/vector.h>

namespace Crypto 
{

    struct UnsignedDivisionResult;
    constexpr size_t STARTING_WORD_SIZE = 512;

    class UnsignedBigInteger 
    {
    public:
        /**
         * @param x 
         */
        UnsignedBigInteger(u32 x) 
        { 
            m_words.append(x); 
        }

        /**
         * @param words 
         */
        explicit UnsignedBigInteger(AK::Vector<u32, STARTING_WORD_SIZE>&& words)
            : m_words(move(words))
        {
        }

        /**
         * @param ptr 
         * @param length 
         */
        explicit UnsignedBigInteger(const u8* ptr, size_t length);

        /// @brief Construct a new Unsigned Big Integer object
        UnsignedBigInteger() { }

        static UnsignedBigInteger create_invalid();

        /**
         * @param data 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger import_data(const Mods::StringView& data) 
        { 
            return import_data((const u8*)data.characters_without_null_termination(), data.length()); 
        }

        /**
         * @param ptr 
         * @param length 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger import_data(const u8* ptr, size_t length)
        {
            return UnsignedBigInteger(ptr, length);
        }

        /**
         * @param remove_leading_zeros 
         * @return size_t 
         */
        size_t export_data(Bytes, bool remove_leading_zeros = false) const;

        /**
         * @param str 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger from_base10(const String& str);

        /**
         * @return String 
         */
        String to_base10() const;

        /**
         * @return const Mods::Vector<u32, STARTING_WORD_SIZE>& 
         */
        const Mods::Vector<u32, STARTING_WORD_SIZE>& words() const 
        { 
            return m_words; 
        }

        void set_to_0();

        /**
         * @param other 
         */
        void set_to(u32 other);

        /**
         * @param other 
         */
        void set_to(const UnsignedBigInteger& other);

        void invalidate()
        {
            m_is_invalid = true;
            m_cached_trimmed_length = {};
        }

        bool is_invalid() const 
        { 
            return m_is_invalid; 
        }

        size_t length() const 
        { 
            return m_words.size(); 
        }

        size_t trimmed_length() const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger plus(const UnsignedBigInteger& other) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger minus(const UnsignedBigInteger& other) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger bitwise_or(const UnsignedBigInteger& other) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger bitwise_and(const UnsignedBigInteger& other) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger bitwise_xor(const UnsignedBigInteger& other) const;

        /**
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger bitwise_not() const;

        /**
         * @param num_bits 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger shift_left(size_t num_bits) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger multiplied_by(const UnsignedBigInteger& other) const;

        /**
         * @param divisor 
         * @return UnsignedDivisionResult 
         */
        UnsignedDivisionResult divided_by(const UnsignedBigInteger& divisor) const;

        /**
         * @param bit_index 
         */
        void set_bit_inplace(size_t bit_index);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void add_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void subtract_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_or_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_and_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param output 
         */
        static void bitwise_xor_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param output 
         */
        static void bitwise_not_without_allocation(const UnsignedBigInteger& left, UnsignedBigInteger& output);

        /**
         * @param number 
         * @param bits_to_shift_by 
         * @param temp_result 
         * @param temp_plus 
         * @param output 
         */
        static void shift_left_without_allocation(const UnsignedBigInteger& number, size_t bits_to_shift_by, UnsignedBigInteger& temp_result, UnsignedBigInteger& temp_plus, UnsignedBigInteger& output);

        /**
         * @param left 
         * @param right 
         * @param temp_shift_result 
         * @param temp_shift_plus 
         * @param temp_shift 
         * @param temp_plus 
         * @param output 
         */
        static void multiply_without_allocation(const UnsignedBigInteger& left, const UnsignedBigInteger& right, UnsignedBigInteger& temp_shift_result, UnsignedBigInteger& temp_shift_plus, UnsignedBigInteger& temp_shift, UnsignedBigInteger& temp_plus, UnsignedBigInteger& output);

        /**
         * @param numerator 
         * @param denominator 
         * @param temp_shift_result 
         * @param temp_shift_plus 
         * @param temp_shift 
         * @param temp_minus 
         * @param quotient 
         * @param remainder 
         */
        static void divide_without_allocation(const UnsignedBigInteger& numerator, const UnsignedBigInteger& denominator, UnsignedBigInteger& temp_shift_result, UnsignedBigInteger& temp_shift_plus, UnsignedBigInteger& temp_shift, UnsignedBigInteger& temp_minus, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder);

        /**
         * @param numerator 
         * @param denominator 
         * @param quotient 
         * @param remainder 
         */
        static void divide_u16_without_allocation(const UnsignedBigInteger& numerator, u32 denominator, UnsignedBigInteger& quotient, UnsignedBigInteger& remainder);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const UnsignedBigInteger& other) const;
        bool operator!=(const UnsignedBigInteger& other) const;
        bool operator<(const UnsignedBigInteger& other) const;

    private:
        /**
         * @param number 
         * @param number_of_words 
         * @param output 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void shift_left_by_n_words(const UnsignedBigInteger& number, size_t number_of_words, UnsignedBigInteger& output);

        /**
         * @param number 
         * @param num_bits 
         * @param result_word_index 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static u32 shift_left_get_one_word(const UnsignedBigInteger& number, size_t num_bits, size_t result_word_index);

        static constexpr size_t BITS_IN_WORD = 32;
        Mods::Vector<u32, STARTING_WORD_SIZE> m_words;

        bool m_is_invalid { false };

        mutable Optional<size_t> m_cached_trimmed_length;
    }; // class UnsignedBigInteger

    struct UnsignedDivisionResult 
    {
        Crypto::UnsignedBigInteger quotient;
        Crypto::UnsignedBigInteger remainder;
    }; // struct

} // namespace Crypto

/**
 * @param stream 
 * @param value 
 * @return const LogStream& 
 */
inline const LogStream&
operator<<(const LogStream& stream, const Crypto::UnsignedBigInteger& value)
{
    if (value.is_invalid()) {
        stream << "Invalid BigInt";
        return stream;
    }

    for (int i = value.length() - 1; i >= 0; --i) {
        stream << value.words()[i] << "|";
    }

    return stream;
}

/**
 * @param string 
 * @param length 
 * @return Crypto::UnsignedBigInteger 
 */
inline Crypto::UnsignedBigInteger
operator""_bigint(const char* string, size_t length)
{
    return Crypto::UnsignedBigInteger::from_base10({ string, length });
}