/**
 * @file asn1.cpp
 * @author Krisna Pranav
 * @brief ASN1
 * @version 6.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/genericlexer.h>
#include <libcrypto/asn1/asn1.h>

namespace Crypto::ASN1 
{

    /**
     * @param kind 
     * @return String 
     */
    String kind_name(Kind kind)
    {
        switch (kind) {
        case Kind::Eol:
            return "EndOfList";
        case Kind::Boolean:
            return "Boolean";
        case Kind::Integer:
            return "Integer";
        case Kind::BitString:
            return "BitString";
        case Kind::OctetString:
            return "OctetString";
        case Kind::Null:
            return "Null";
        case Kind::ObjectIdentifier:
            return "ObjectIdentifier";
        case Kind::IA5String:
            return "IA5String";
        case Kind::PrintableString:
            return "PrintableString";
        case Kind::Utf8String:
            return "UTF8String";
        case Kind::UTCTime:
            return "UTCTime";
        case Kind::GeneralizedTime:
            return "GeneralizedTime";
        case Kind::Sequence:
            return "Sequence";
        case Kind::Set:
            return "Set";
        }

        return "InvalidKind";
    }

    /**
     * @param class_ 
     * @return String 
     */
    String class_name(Class class_)
    {
        switch (class_) {
        case Class::Application:
            return "Application";
        case Class::Context:
            return "Context";
        case Class::Private:
            return "Private";
        case Class::Universal:
            return "Universal";
        }

        return "InvalidClass";
    }

    /**
     * @param type 
     * @return String 
     */
    String type_name(Type type)
    {
        switch (type) {
        case Type::Constructed:
            return "Constructed";
        case Type::Primitive:
            return "Primitive";
        }

        return "InvalidType";
    }

    /**
     * @param time 
     * @return Optional<Core::DateTime> 
     */
    Optional<Core::DateTime> parse_utc_time(StringView time)
    {
        GenericLexer lexer(time);
        auto year_in_century = lexer.consume(2).to_uint();
        auto month = lexer.consume(2).to_uint();
        auto day = lexer.consume(2).to_uint();
        auto hour = lexer.consume(2).to_uint();
        auto minute = lexer.consume(2).to_uint();
        Optional<unsigned> seconds, offset_hours, offset_minutes;
        [[maybe_unused]] bool negative_offset = false;
        
        if (!lexer.next_is('Z')) {
            if (!lexer.next_is(is_any_of("+-"))) {
                seconds = lexer.consume(2).to_uint();
                if (!seconds.has_value()) {
                    return {};
                }
            }

            if (lexer.next_is(is_any_of("+-"))) {
                negative_offset = lexer.consume() == '-';
                offset_hours = lexer.consume(2).to_uint();
                offset_minutes = lexer.consume(2).to_uint();
                if (!offset_hours.has_value() || !offset_minutes.has_value()) {
                    return {};
                }
            } else {
                lexer.consume();
            }
        } else {
            lexer.consume();
        }

        if (!year_in_century.has_value() || !month.has_value() || !day.has_value() || !hour.has_value() || !minute.has_value()) {
            return {};
        }

        auto full_year = year_in_century.value();
        full_year += (full_year < 50) ? 2000 : 1900;
        auto full_seconds = seconds.value_or(0);

        if (offset_hours.has_value() || offset_minutes.has_value())
            dbgln("FIXME: Implement UTCTime with offset!");

        return Core::DateTime::create(full_year, month.value(), day.value(), hour.value(), minute.value(), full_seconds);
    }

    /**
     * @param time 
     * @return Optional<Core::DateTime> 
     */
    Optional<Core::DateTime> parse_generalized_time(StringView time)
    {
        GenericLexer lexer(time);
        auto year = lexer.consume(4).to_uint();
        auto month = lexer.consume(2).to_uint();
        auto day = lexer.consume(2).to_uint();
        auto hour = lexer.consume(2).to_uint();
        Optional<unsigned> minute, seconds, milliseconds, offset_hours, offset_minutes;
        [[maybe_unused]] bool negative_offset = false;
        if (!lexer.is_eof()) {
            if (lexer.consume_specific('Z'))
                goto done_parsing;

            if (!lexer.next_is(is_any_of("+-"))) {
                minute = lexer.consume(2).to_uint();
                if (!minute.has_value()) {
                    return {};
                }
                if (lexer.consume_specific('Z'))
                    goto done_parsing;
            }

            if (!lexer.next_is(is_any_of("+-"))) {
                seconds = lexer.consume(2).to_uint();
                if (!seconds.has_value()) {
                    return {};
                }
                if (lexer.consume_specific('Z'))
                    goto done_parsing;
            }

            if (lexer.consume_specific('.')) {
                milliseconds = lexer.consume(3).to_uint();
                if (!milliseconds.has_value()) {
                    return {};
                }
                if (lexer.consume_specific('Z'))
                    goto done_parsing;
            }

            if (lexer.next_is(is_any_of("+-"))) {
                negative_offset = lexer.consume() == '-';
                offset_hours = lexer.consume(2).to_uint();
                offset_minutes = lexer.consume(2).to_uint();
                if (!offset_hours.has_value() || !offset_minutes.has_value()) {
                    return {};
                }
            } else {
                lexer.consume();
            }
        }

    done_parsing:;

        if (!year.has_value() || !month.has_value() || !day.has_value() || !hour.has_value()) {
            return {};
        }

        if (offset_hours.has_value() || offset_minutes.has_value())
            dbgln("FIXME: Implement GeneralizedTime with offset!");

        return Core::DateTime::create(year.value(), month.value(), day.value(), hour.value(), minute.value_or(0), seconds.value_or(0));
    }

} // namespace Crypto::ASN1 
