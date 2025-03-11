/**
 * @file unsignedbiginteger.h
 * @author Krisna Pranav
 * @brief unsigned big integer
 * @version 6.0
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/span.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Crypto 
{

    struct UnsignedDivisionResult;
    constexpr size_t STARTING_WORD_SIZE = 32;

    class UnsignedBigInteger 
    {
    public:
        using Word = u32;
        static constexpr size_t BITS_IN_WORD = 32;

        /**
         * @brief Construct a new UnsignedBigInteger object
         * 
         * @param x 
         */
        UnsignedBigInteger(Word x) 
        { 
            m_words.append(x); 
        }

        /**
         * @brief Construct a new UnsignedBigInteger object
         * 
         * @param words 
         */
        explicit UnsignedBigInteger(Vector<Word, STARTING_WORD_SIZE>&& words)
            : m_words(move(words))
        {
        }

        /**
         * @brief Construct a new UnsignedBigInteger object
         * 
         * @param ptr 
         * @param length 
         */
        explicit UnsignedBigInteger(u8 const* ptr, size_t length);

        /**
         * @brief Construct a new UnsignedBigInteger object
         * 
         */
        UnsignedBigInteger() = default;

        static UnsignedBigInteger create_invalid();

        /**
         * @param data 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger import_data(StringView data) 
        { 
            return import_data((u8 const*)data.characters_without_null_termination(), data.length()); 
        }

        /**
         * @param ptr 
         * @param length 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger import_data(u8 const* ptr, size_t length)
        {
            return UnsignedBigInteger(ptr, length);
        }

        /**
         * @brief Create a from object
         * 
         * @param value 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger create_from(u64 value)
        {
            VERIFY(sizeof(Word) == 4);
            UnsignedBigInteger integer;
            integer.m_words.resize(2);
            integer.m_words[0] = static_cast<Word>(value & 0xFFFFFFFF);
            integer.m_words[1] = static_cast<Word>((value >> 32) & 0xFFFFFFFF);
            return integer;
        }

        /**
         * @param remove_leading_zeros 
         * @return size_t 
         */
        size_t export_data(Bytes, bool remove_leading_zeros = false) const;

        /**
         * @param N 
         * @param str 
         * @return UnsignedBigInteger 
         */
        static UnsignedBigInteger from_base(u16 N, StringView str);

        /**
         * @param N 
         * @return String 
         */
        String to_base(u16 N) const;

        u64 to_u64() const;
        double to_double() const;

        /**
         * @return Vector<Word, STARTING_WORD_SIZE> const& 
         */
        Vector<Word, STARTING_WORD_SIZE> const& words() const 
        { 
            return m_words; 
        }

        void set_to_0();

        /**
         * @brief Set the to object
         * 
         * @param other 
         */
        void set_to(Word other);

        /**
         * @brief Set the to object
         * 
         * @param other 
         */
        void set_to(UnsignedBigInteger const& other);

        void invalidate()
        {
            m_is_invalid = true;
            m_cached_trimmed_length = {};
            m_cached_hash = 0;
        }

        bool is_zero() const;

        bool is_odd() const 
        { 
            return m_words.size() && (m_words[0] & 1); 
        }
        
        bool is_invalid() const 
        { 
            return m_is_invalid; 
        }

        /**
         * @return size_t 
         */
        size_t length() const 
        { 
            return m_words.size(); 
        }
        
        size_t trimmed_length() const;

        void clamp_to_trimmed_length();

        /**
         * @param num_words 
         */
        void resize_with_leading_zeros(size_t num_words);

        size_t one_based_index_of_highest_set_bit() const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger plus(UnsignedBigInteger const& other) const;
        UnsignedBigInteger minus(UnsignedBigInteger const& other) const;
        UnsignedBigInteger bitwise_or(UnsignedBigInteger const& other) const;
        UnsignedBigInteger bitwise_and(UnsignedBigInteger const& other) const;
        UnsignedBigInteger bitwise_xor(UnsignedBigInteger const& other) const;
        UnsignedBigInteger bitwise_not_fill_to_one_based_index(size_t) const;

        /**
         * @param num_bits 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger shift_left(size_t num_bits) const;

        /**
         * @param other 
         * @return UnsignedBigInteger 
         */
        UnsignedBigInteger multiplied_by(UnsignedBigInteger const& other) const;

        /**
         * @param divisor 
         * @return UnsignedDivisionResult 
         */
        UnsignedDivisionResult divided_by(UnsignedBigInteger const& divisor) const;

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
        bool operator==(UnsignedBigInteger const& other) const;
        bool operator!=(UnsignedBigInteger const& other) const;
        bool operator<(UnsignedBigInteger const& other) const;
        bool operator>(UnsignedBigInteger const& other) const;
        bool operator>=(UnsignedBigInteger const& other) const;

    private:
        friend class UnsignedBigIntegerAlgorithms;
        
        Vector<Word, STARTING_WORD_SIZE> m_words;

        mutable u32 m_cached_hash { 0 };

        bool m_is_invalid { false };

        mutable Optional<size_t> m_cached_trimmed_length;
    }; // class UnsignedBigInteger

    struct UnsignedDivisionResult {
        Crypto::UnsignedBigInteger quotient;
        Crypto::UnsignedBigInteger remainder;
    }; // struct UnsignedDivisionResult

} // namespace Crypto

template<>
struct Mods::Formatter<Crypto::UnsignedBigInteger> : Formatter<StringView> {
    ErrorOr<void> format(FormatBuilder&, Crypto::UnsignedBigInteger const&);
};

/**
 * @param string 
 * @param length 
 * @return Crypto::UnsignedBigInteger 
 */
inline Crypto::UnsignedBigInteger
operator""_bigint(char const* string, size_t length)
{
    return Crypto::UnsignedBigInteger::from_base(10, { string, length });
}
