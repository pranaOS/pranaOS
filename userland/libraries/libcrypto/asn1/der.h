/**
 * @file der.h
 * @author Krisna Pranav
 * @brief DER
 * @version 6.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bitmapview.h>
#include <mods/result.h>
#include <mods/types.h>
#include <libcrypto/asn1/asn1.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto::ASN1 
{

    enum class DecodeError {
        NoInput,
        NonConformingType,
        EndOfStream,
        NotEnoughData,
        EnteringNonConstructedTag,
        LeavingMainContext,
        InvalidInputFormat,
        Overflow,
        UnsupportedFormat,
    }; // enum class DecodeError

    class BitStringView 
    {
    public:
        /**
         * @brief Construct a new BitStringView object
         * 
         * @param data 
         * @param unused_bits 
         */
        BitStringView(ReadonlyBytes data, size_t unused_bits)
            : m_data(data)
            , m_unused_bits(unused_bits)
        {
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes raw_bytes() const
        {
            VERIFY(m_unused_bits == 0);
            return m_data;
        }

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool get(size_t index)
        {
            if (index >= 8 * m_data.size() - m_unused_bits)
                return false;
            return 0 != (m_data[index / 8] & (1u << (7 - (index % 8))));
        }

    private:
        ReadonlyBytes m_data;
        size_t m_unused_bits;
    }; // class BitStringView 

    class Decoder 
    {
    public:
        /**
         * @brief Construct a new Decoder object
         * 
         * @param data 
         */
        Decoder(ReadonlyBytes data)
        {
            m_stack.append(data);
        }

        /**
         * @return Result<Tag, DecodeError> 
         */
        Result<Tag, DecodeError> peek();

        bool eof() const;

        /**
         * @tparam ValueType 
         */
        template<typename ValueType>
        struct TaggedValue {
            Tag tag;
            ValueType value;
        }; // struct TaggedValue

        /**
         * @return Optional<DecodeError> 
         */
        Optional<DecodeError> drop()
        {
            if (m_stack.is_empty())
                return DecodeError::NoInput;

            if (eof())
                return DecodeError::EndOfStream;

            auto previous_position = m_stack;

            auto tag_or_error = peek();
            if (tag_or_error.is_error()) {
                m_stack = move(previous_position);
                return tag_or_error.error();
            }

            auto length_or_error = read_length();
            if (length_or_error.is_error()) {
                m_stack = move(previous_position);
                return length_or_error.error();
            }

            auto length = length_or_error.value();

            auto bytes_result = read_bytes(length);
            if (bytes_result.is_error()) {
                m_stack = move(previous_position);
                return bytes_result.error();
            }

            m_current_tag.clear();
            return {};
        }

        /**
         * @tparam ValueType 
         * @param class_override 
         * @param kind_override 
         * @return Result<ValueType, DecodeError> 
         */
        template<typename ValueType>
        Result<ValueType, DecodeError> read(Optional<Class> class_override = {}, Optional<Kind> kind_override = {})
        {
            if (m_stack.is_empty())
                return DecodeError::NoInput;

            if (eof())
                return DecodeError::EndOfStream;

            auto previous_position = m_stack;

            auto tag_or_error = peek();
            if (tag_or_error.is_error()) {
                m_stack = move(previous_position);
                return tag_or_error.error();
            }

            auto length_or_error = read_length();
            if (length_or_error.is_error()) {
                m_stack = move(previous_position);
                return length_or_error.error();
            }

            auto tag = tag_or_error.value();
            auto length = length_or_error.value();

            auto value_or_error = read_value<ValueType>(class_override.value_or(tag.class_), kind_override.value_or(tag.kind), length);
            if (value_or_error.is_error()) {
                m_stack = move(previous_position);
                return value_or_error.error();
            }

            m_current_tag.clear();

            return value_or_error.release_value();
        }

        Optional<DecodeError> enter();
        Optional<DecodeError> leave();

    private:
        /**
         * @tparam ValueType 
         * @tparam DecodedType 
         * @param value 
         * @return Result<ValueType, DecodeError> 
         */
        template<typename ValueType, typename DecodedType>
        Result<ValueType, DecodeError> with_type_check(DecodedType&& value)
        {
            if constexpr (requires { ValueType { value }; })
                return ValueType { value };

            return DecodeError::NonConformingType;
        }

        /**
         * @tparam ValueType 
         * @tparam DecodedType 
         * @param value_or_error 
         * @return Result<ValueType, DecodeError> 
         */
        template<typename ValueType, typename DecodedType>
        Result<ValueType, DecodeError> with_type_check(Result<DecodedType, DecodeError>&& value_or_error)
        {
            if (value_or_error.is_error())
                return value_or_error.error();

            if constexpr (IsSame<ValueType, bool> && !IsSame<DecodedType, bool>) {
                return DecodeError::NonConformingType;
            } else {
                auto&& value = value_or_error.value();
                if constexpr (requires { ValueType { value }; })
                    return ValueType { value };
            }

            return DecodeError::NonConformingType;
        }

        /**
         * @tparam ValueType 
         * @param klass 
         * @param kind 
         * @param length 
         * @return Result<ValueType, DecodeError> 
         */
        template<typename ValueType>
        Result<ValueType, DecodeError> read_value(Class klass, Kind kind, size_t length)
        {
            auto data_or_error = read_bytes(length);
            if (data_or_error.is_error())
                return data_or_error.error();
            auto data = data_or_error.value();

            if (klass != Class::Universal)
                return with_type_check<ValueType>(data);

            if (kind == Kind::Boolean)
                return with_type_check<ValueType>(decode_boolean(data));

            if (kind == Kind::Integer)
                return with_type_check<ValueType>(decode_arbitrary_sized_integer(data));

            if (kind == Kind::OctetString)
                return with_type_check<ValueType>(decode_octet_string(data));

            if (kind == Kind::Null)
                return with_type_check<ValueType>(decode_null(data));

            if (kind == Kind::ObjectIdentifier)
                return with_type_check<ValueType>(decode_object_identifier(data));

            if (kind == Kind::PrintableString || kind == Kind::IA5String || kind == Kind::UTCTime)
                return with_type_check<ValueType>(decode_printable_string(data));

            if (kind == Kind::Utf8String)
                return with_type_check<ValueType>(StringView { data.data(), data.size() });

            if (kind == Kind::BitString)
                return with_type_check<ValueType>(decode_bit_string(data));

            return with_type_check<ValueType>(data);
        }

        Result<Tag, DecodeError> read_tag();
        Result<size_t, DecodeError> read_length();
        Result<u8, DecodeError> read_byte();

        /**
         * @param length 
         * @return Result<ReadonlyBytes, DecodeError> 
         */
        Result<ReadonlyBytes, DecodeError> read_bytes(size_t length);

        static Result<bool, DecodeError> decode_boolean(ReadonlyBytes);
        static Result<UnsignedBigInteger, DecodeError> decode_arbitrary_sized_integer(ReadonlyBytes);
        static Result<StringView, DecodeError> decode_octet_string(ReadonlyBytes);
        static Result<std::nullptr_t, DecodeError> decode_null(ReadonlyBytes);
        static Result<Vector<int>, DecodeError> decode_object_identifier(ReadonlyBytes);
        static Result<StringView, DecodeError> decode_printable_string(ReadonlyBytes);
        static Result<BitStringView, DecodeError> decode_bit_string(ReadonlyBytes);

        Vector<ReadonlyBytes> m_stack;
        Optional<Tag> m_current_tag;
    }; // class Decoder 

    /**
     * @param indent 
     */
    void pretty_print(Decoder&, OutputStream&, int indent = 0);

} // namespace Crypto::ASN1

/**
 * @tparam  
 */
template<>
struct Mods::Formatter<Crypto::ASN1::DecodeError> : Formatter<StringView> {
    ErrorOr<void> format(FormatBuilder&, Crypto::ASN1::DecodeError);
};
