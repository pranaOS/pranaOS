/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/CharacterTypes.h>
#include <base/DateTimeLexer.h>
#include <libjs/runtime/IteratorOperations.h>
#include <libjs/runtime/temporal/AbstractOperations.h>
#include <libjs/runtime/temporal/Duration.h>
#include <libjs/runtime/temporal/PlainDate.h>
#include <libjs/runtime/temporal/PlainTime.h>
#include <libjs/runtime/temporal/TimeZone.h>

namespace JS::Temporal {

static Optional<OptionType> to_option_type(Value value)
{
    if (value.is_boolean())
        return OptionType::Boolean;
    if (value.is_string())
        return OptionType::String;
    if (value.is_number())
        return OptionType::Number;
    return {};
}

MarkedValueList iterable_to_list_of_type(GlobalObject& global_object, Value items, Vector<OptionType> const& element_types)
{
    auto& vm = global_object.vm();
    auto& heap = global_object.heap();

    auto iterator_record = get_iterator(global_object, items, IteratorHint::Sync);
    if (vm.exception())
        return MarkedValueList { heap };

    MarkedValueList values(heap);

    auto next = true;
    while (next) {
        auto* iterator_result = iterator_step(global_object, *iterator_record);
        if (vm.exception())
            return MarkedValueList { heap };
        next = iterator_result;

        if (next) {
            auto next_value = iterator_value(global_object, *iterator_result);
            if (vm.exception())
                return MarkedValueList { heap };

            if (auto type = to_option_type(next_value); !type.has_value() || !element_types.contains_slow(*type)) {

                vm.throw_exception<TypeError>(global_object, ErrorType::FixmeAddAnErrorString);

                iterator_close(*iterator_record);
                return MarkedValueList { heap };
            }
            
            values.append(next_value);
        }
    }

    return values;
}

Object* get_options_object(GlobalObject& global_object, Value options)
{
    auto& vm = global_object.vm();

    if (options.is_undefined()) {

        return Object::create(global_object, nullptr);
    }

    if (options.is_object()) {

        return &options.as_object();
    }

    vm.throw_exception<TypeError>(global_object, ErrorType::NotAnObject, "Options");
    return {};
}

Value get_option(GlobalObject& global_object, Object& options, String const& property, Vector<OptionType> const& types, Vector<StringView> const& values, Value fallback)
{
    auto& vm = global_object.vm();

    auto value = options.get(property);
    if (vm.exception())
        return {};

    if (value.is_undefined())
        return fallback;

    OptionType type;
    
    if (auto value_type = to_option_type(value); value_type.has_value() && types.contains_slow(*value_type)) {
        type = *value_type;
    }

    else {
        type = types.last();
    }

    if (type == OptionType::Boolean) {

        value = Value(value.to_boolean());
    }

    else if (type == OptionType::Number) {

        value = value.to_number(global_object);
        if (vm.exception())
            return {};

        if (value.is_nan()) {
            vm.throw_exception<RangeError>(global_object, ErrorType::OptionIsNotValidValue, vm.names.NaN.as_string(), property);
            return {};
        }
    }    
    else {

        value = value.to_primitive_string(global_object);
        if (vm.exception())
            return {};
    }

    if (!values.is_empty()) {
        VERIFY(value.is_string());

        if (!values.contains_slow(value.as_string().string())) {
            vm.throw_exception<RangeError>(global_object, ErrorType::OptionIsNotValidValue, value.as_string().string(), property);
            return {};
        }
    }

    return value;
}

Optional<String> to_temporal_overflow(GlobalObject& global_object, Object& normalized_options)
{
    auto& vm = global_object.vm();

    auto option = get_option(global_object, normalized_options, "overflow", { OptionType::String }, { "constrain"sv, "reject"sv }, js_string(vm, "constrain"));
    if (vm.exception())
        return {};

    VERIFY(option.is_string());
    return option.as_string().string();
}

Optional<String> to_temporal_rounding_mode(GlobalObject& global_object, Object& normalized_options, String const& fallback)
{
    auto& vm = global_object.vm();

    auto option = get_option(global_object, normalized_options, "roundingMode", { OptionType::String }, { "ceil"sv, "floor"sv, "trunc"sv, "halfExpand"sv }, js_string(vm, fallback));
    if (vm.exception())
        return {};

    VERIFY(option.is_string());
    return option.as_string().string();
}

u64 to_temporal_rounding_increment(GlobalObject& global_object, Object& normalized_options, Optional<double> dividend, bool inclusive)
{
    auto& vm = global_object.vm();

    double maximum;
    if (!dividend.has_value()) {
        maximum = INFINITY;
    }

    else if (inclusive) {
        maximum = *dividend;
    }
    else if (*dividend > 1) {

        maximum = *dividend - 1;
    }

    else {
        maximum = 1;
    }

    auto increment_value = get_option(global_object, normalized_options, "roundingIncrement", { OptionType::Number }, {}, Value(1));
    if (vm.exception())
        return {};
    VERIFY(increment_value.is_number());
    auto increment = increment_value.as_double();


    if (increment < 1 || increment > maximum) {
        vm.throw_exception<RangeError>(global_object, ErrorType::OptionIsNotValidValue, increment, "roundingIncrement");
        return {};
    }

    auto floored_increment = static_cast<u64>(increment);

    if (dividend.has_value() && static_cast<u64>(*dividend) % floored_increment != 0) {

        vm.throw_exception<RangeError>(global_object, ErrorType::OptionIsNotValidValue, increment, "roundingIncrement");
        return {};
    }

    return floored_increment;
}

static HashMap<StringView, StringView> plural_to_singular_units = {
    { "years"sv, "year"sv },
    { "months"sv, "month"sv },
    { "weeks"sv, "week"sv },
    { "days"sv, "day"sv },
    { "hours"sv, "hour"sv },
    { "minutes"sv, "minute"sv },
    { "seconds"sv, "second"sv },
    { "milliseconds"sv, "millisecond"sv },
    { "microseconds"sv, "microsecond"sv },
    { "nanoseconds"sv, "nanosecond"sv }
};


Optional<String> to_smallest_temporal_unit(GlobalObject& global_object, Object& normalized_options, Vector<StringView> const& disallowed_units, Optional<String> fallback)
{
    auto& vm = global_object.vm();

    auto smallest_unit_value = get_option(global_object, normalized_options, "smallestUnit"sv, { OptionType::String }, { "year"sv, "years"sv, "month"sv, "months"sv, "week"sv, "weeks"sv, "day"sv, "days"sv, "hour"sv, "hours"sv, "minute"sv, "minutes"sv, "second"sv, "seconds"sv, "millisecond"sv, "milliseconds"sv, "microsecond"sv, "microseconds"sv, "nanosecond"sv, "nanoseconds"sv }, fallback.has_value() ? js_string(vm, *fallback) : js_undefined());
    if (vm.exception())
        return {};

    if (smallest_unit_value.is_undefined())
        return {};
    VERIFY(smallest_unit_value.is_string());
    auto smallest_unit = smallest_unit_value.as_string().string();

    if (auto singular_unit = plural_to_singular_units.get(smallest_unit); singular_unit.has_value()) {
        smallest_unit = singular_unit.value();
    }

    if (disallowed_units.contains_slow(smallest_unit)) {
    
        vm.throw_exception<RangeError>(global_object, ErrorType::OptionIsNotValidValue, smallest_unit, "smallestUnit");
        return {};
    }

    return smallest_unit;
}

double constrain_to_range(double x, double minimum, double maximum)
{
    return min(max(x, minimum), maximum);
}

BigInt* round_number_to_increment(GlobalObject& global_object, BigInt const& x, u64 increment, String const& rounding_mode)
{
    auto& heap = global_object.heap();

    VERIFY(rounding_mode == "ceil" || rounding_mode == "floor" || rounding_mode == "trunc" || rounding_mode == "halfExpand");

    if (increment == 1)
        return js_bigint(heap, x.big_integer());

    auto increment_big_int = Crypto::UnsignedBigInteger::create_from(increment);
    
    auto division_result = x.big_integer().divided_by(increment_big_int);

    if (division_result.remainder == Crypto::UnsignedBigInteger { 0 })
        return js_bigint(heap, x.big_integer());

    Crypto::SignedBigInteger rounded = move(division_result.quotient);

    if (rounding_mode == "ceil") {
        
        if (!division_result.remainder.is_negative())
            rounded = rounded.plus(Crypto::UnsignedBigInteger { 1 });
    }
    
    else if (rounding_mode == "floor") {

        if (division_result.remainder.is_negative())
            rounded = rounded.minus(Crypto::UnsignedBigInteger { 1 });
    }
    else if (rounding_mode == "trunc") {
        // a. Let rounded be the integral part of quotient, removing any fractional digits.
        // NOTE: This is a no-op
    }

    else {

        if (division_result.remainder.multiplied_by(Crypto::UnsignedBigInteger { 2 }).unsigned_value() >= increment_big_int) {
            if (division_result.remainder.is_negative())
                rounded = rounded.minus(Crypto::UnsignedBigInteger { 1 });
            else
                rounded = rounded.plus(Crypto::UnsignedBigInteger { 1 });
        }
    }

    return js_bigint(heap, rounded.multiplied_by(increment_big_int));
}

Optional<ISODateTime> parse_iso_date_time(GlobalObject& global_object, [[maybe_unused]] String const& iso_string)
{
    auto& vm = global_object.vm();

    Optional<StringView> year_part;
    Optional<StringView> month_part;
    Optional<StringView> day_part;
    Optional<StringView> hour_part;
    Optional<StringView> minute_part;
    Optional<StringView> second_part;
    Optional<StringView> fraction_part;
    Optional<StringView> calendar_part;
    TODO();

    String normalized_year;
    if (year_part.has_value() && year_part->starts_with("\xE2\x88\x92"sv))
        normalized_year = String::formatted("-{}", year_part->substring_view(3));
    else
        normalized_year = year_part.value_or("");

    i32 year = Value(js_string(vm, normalized_year)).to_integer_or_infinity(global_object);

    u8 month;
    
    if (!month_part.has_value()) {

        month = 1;
    }

    else {
        
        month = *month_part->to_uint<u8>();
    }

    u8 day;

    if (!day_part.has_value()) {

        day = 1;
    }

    else {
        
        day = *day_part->to_uint<u8>();
    }

    u8 hour = hour_part->to_uint<u8>().value_or(0);

    u8 minute = minute_part->to_uint<u8>().value_or(0);

    u8 second = second_part->to_uint<u8>().value_or(0);

    if (second == 60) {

        second = 59;
    }

    u16 millisecond;
    u16 microsecond;
    u16 nanosecond;
    if (fraction_part.has_value()) {
        auto fraction = String::formatted("{}000000000", *fraction_part);
        
        millisecond = *fraction.substring(0, 3).to_uint<u16>();
        microsecond = *fraction.substring(3, 3).to_uint<u16>();
        nanosecond = *fraction.substring(6, 3).to_uint<u16>();
    }

    else {
        millisecond = 0;
        microsecond = 0;
        nanosecond = 0;
    }

    if (!is_valid_iso_date(year, month, day)) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidISODate);
        return {};
    }

    if (!is_valid_time(hour, minute, second, millisecond, microsecond, nanosecond)) {
        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidTime);
        return {};
    }

    return ISODateTime { .year = year, .month = month, .day = day, .hour = hour, .minute = minute, .second = second, .millisecond = millisecond, .microsecond = microsecond, .nanosecond = nanosecond, .calendar = calendar_part.has_value() ? *calendar_part : Optional<String>() };
}


Optional<TemporalInstant> parse_temporal_instant_string(GlobalObject& global_object, String const& iso_string)
{
    auto& vm = global_object.vm();



    auto result = parse_iso_date_time(global_object, iso_string);
    
    auto time_zone_result = parse_temporal_time_zone_string(global_object, iso_string);
    if (vm.exception())
        return {};

    VERIFY(time_zone_result->offset.has_value());

    return TemporalInstant { .year = result->year, .month = result->month, .day = result->day, .hour = result->hour, .minute = result->minute, .second = result->second, .millisecond = result->millisecond, .microsecond = result->microsecond, .nanosecond = result->nanosecond, .time_zone_offset = move(time_zone_result->offset) };
}

Optional<String> parse_temporal_calendar_string([[maybe_unused]] GlobalObject& global_object, [[maybe_unused]] String const& iso_string)
{

    Optional<StringView> id_part;
    TODO();

    if (!id_part.has_value()) {

        return "iso8601";
    }

    return id_part.value();
}

Optional<TemporalDate> parse_temporal_date_string(GlobalObject& global_object, String const& iso_string)
{
    auto& vm = global_object.vm();

    auto result = parse_iso_date_time(global_object, iso_string);
    if (vm.exception())
        return {};

    return TemporalDate { .year = result->year, .month = result->month, .day = result->day, .calendar = move(result->calendar) };
}

Optional<TemporalDuration> parse_temporal_duration_string(GlobalObject& global_object, String const& iso_string)
{
    (void)global_object;
    (void)iso_string;
    TODO();
}

Optional<TemporalTimeZone> parse_temporal_time_zone_string(GlobalObject& global_object, [[maybe_unused]] String const& iso_string)
{
    auto& vm = global_object.vm();

    Optional<StringView> z_part;
    Optional<StringView> sign_part;
    Optional<StringView> hours_part;
    Optional<StringView> minutes_part;
    Optional<StringView> seconds_part;
    Optional<StringView> fraction_part;
    Optional<StringView> name_part;
    TODO();

    if (z_part.has_value()) {

        return TemporalTimeZone { .z = true, .offset = "+00:00", .name = {} };
    }

    Optional<String> offset;
    
    if (!hours_part.has_value()) {

    }

    else {

        VERIFY(sign_part.has_value());

        u8 hours = Value(js_string(vm, *hours_part)).to_integer_or_infinity(global_object);

        u8 sign;

        if (sign_part->is_one_of("-", "\u2212")) {
            
            sign = -1;
        }

        else {
            
            sign = 1;
        }

        u8 minutes = Value(js_string(vm, minutes_part.value_or(""sv))).to_integer_or_infinity(global_object);

        u8 seconds = Value(js_string(vm, seconds_part.value_or(""sv))).to_integer_or_infinity(global_object);

        i32 nanoseconds;

        if (fraction_part.has_value()) {

            auto fraction = String::formatted("{}000000000", *fraction_part);

            nanoseconds = Value(js_string(vm, fraction.substring(0, 9))).to_integer_or_infinity(global_object);
        }
        else {
            nanoseconds = 0;
        }

        auto offset_nanoseconds = sign * (((hours * 60 + minutes) * 60 + seconds) * 1000000000 + nanoseconds);

        offset = format_time_zone_offset_string(offset_nanoseconds);
    }

    Optional<String> name;

    if (name_part.has_value()) {

        if (!is_valid_time_zone_name(*name_part)) {
            vm.throw_exception<RangeError>(global_object, ErrorType::TemporalInvalidTimeZoneName);
            return {};
        }

        name = canonicalize_time_zone_name(*name_part);
    }

    return TemporalTimeZone { .z = false, .offset = offset, .name = name };
}

double to_positive_integer_or_infinity(GlobalObject& global_object, Value argument)
{
    auto& vm = global_object.vm();

    auto integer = argument.to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    if (Value(integer).is_negative_infinity()) {

        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalPropertyMustBePositiveInteger);
        return {};
    }

    if (integer <= 0) {

        vm.throw_exception<RangeError>(global_object, ErrorType::TemporalPropertyMustBePositiveInteger);
        return {};
    }

    return integer;
}

Object* prepare_temporal_fields(GlobalObject& global_object, Object& fields, Vector<String> const& field_names, Vector<StringView> const& required_fields)
{
    auto& vm = global_object.vm();

    auto* result = Object::create(global_object, global_object.object_prototype());
    VERIFY(result);

    for (auto& property : field_names) {

        auto value = fields.get(property);
        if (vm.exception())
            return {};

        if (value.is_undefined()) {

            if (required_fields.contains_slow(property)) {
                vm.throw_exception<TypeError>(global_object, ErrorType::TemporalMissingRequiredProperty, property);
                return {};
            }

            if (property.is_one_of("hour", "minute", "second", "millisecond", "microsecond", "nanosecond")) {

                value = Value(0);
            }
        }

        else {
            if (property.is_one_of("year", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond", "eraYear")) {
                value = Value(value.to_integer_or_infinity(global_object));
                if (vm.exception())
                    return {};
            } else if (property.is_one_of("month", "day")) {
                value = Value(to_positive_integer_or_infinity(global_object, value));
                if (vm.exception())
                    return {};
            } else if (property.is_one_of("monthCode", "offset", "era")) {
                value = value.to_primitive_string(global_object);
                if (vm.exception())
                    return {};
            }
        }

        result->create_data_property_or_throw(property, value);
    }

    return result;
}

}
