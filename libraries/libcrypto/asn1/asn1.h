/**
 * @file asn1.h
 * @author Krisna Pranav
 * @brief ASN 1
 * @version 6.0
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/types.h>
#include <libcore/datetime.h>
#include <libcrypto/bigint/unsignedbiginteger.h>

namespace Crypto::ASN1 
{
    enum class Kind : u8
    {
        Eol,
        Boolean = 0x01,
        Integer = 0x02,
        BitString = 0x03,
        OctetString = 0x04,
        Null = 0x05,
        ObjectIdentifier = 0x06,
        IA5String = 0x16,
        PrintableString = 0x13,
        Utf8String = 0x0c,
        UTCTime = 0x017,
        GeneralizedTime = 0x018,
        Sequence = 0x10,
        Set = 0x11,
    }; // enum class Kind : u8

    enum class Class : u8 
    {
        Universal = 0,
        Application = 0x40,
        Context = 0x80,
        Private = 0xc0,
    }; // enum class Class : u8 

    enum class Type : u8 
    {
        Primitive = 0,
        Constructed = 0x20,
    }; // enum class Type : u8 

    struct Tag 
    {
        Kind kind;
        Class class_;
        Type type;
    }; // struct Tag

    String kind_name(Kind);
    String class_name(Class);
    String type_name(Type);

    /**
     * @return Optional<Core::DateTime> 
     */
    Optional<Core::DateTime> parse_utc_time(StringView);

    /**
     * @return Optional<Core::DateTime> 
     */
    Optional<Core::DateTime> parse_generalized_time(StringView);

} // namespace Crypto::ASN1