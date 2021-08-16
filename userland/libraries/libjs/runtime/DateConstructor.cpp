/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include <base/CharacterTypes.h>
#include <base/GenericLexer.h>
#include <libcore/DateTime.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Date.h>
#include <libjs/runtime/DateConstructor.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/VM.h>
#include <sys/time.h>
#include <time.h>

namespace JS {

static Value parse_simplified_iso8601(const String& iso_8601)
{
    GenericLexer lexer(iso_8601);
    auto lex_n_digits = [&](size_t n, Optional<int>& out) {
        if (lexer.tell_remaining() < n)
            return false;
        int r = 0;
        for (size_t i = 0; i < n; ++i) {
            char ch = lexer.consume();
            if (!is_ascii_digit(ch))
                return false;
            r = 10 * r + ch - '0';
        }
        out = r;
        return true;
    };

    Optional<int> year;
    Optional<int> month;
    Optional<int> day;
    Optional<int> hours;
    Optional<int> minutes;
    Optional<int> seconds;
    Optional<int> milliseconds;
    Optional<char> timezone;
    Optional<int> timezone_hours;
    Optional<int> timezone_minutes;

    auto lex_year = [&]() {
        if (lexer.consume_specific('+'))
            return lex_n_digits(6, year);
        if (lexer.consume_specific('-')) {
            Optional<int> absolute_year;
            if (!lex_n_digits(6, absolute_year))
                return false;
            year = -absolute_year.value();
            return true;
        }
        return lex_n_digits(4, year);
    };
    auto lex_month = [&]() { return lex_n_digits(2, month) && *month >= 1 && *month <= 12; };
    auto lex_day = [&]() { return lex_n_digits(2, day) && *day >= 1 && *day <= 31; };
    auto lex_date = [&]() { return lex_year() && (!lexer.consume_specific('-') || (lex_month() && (!lexer.consume_specific('-') || lex_day()))); };

    auto lex_hours_minutes = [&](Optional<int>& out_h, Optional<int>& out_m) {
        Optional<int> h;
        Optional<int> m;
        if (lex_n_digits(2, h) && *h >= 0 && *h <= 24 && lexer.consume_specific(':') && lex_n_digits(2, m) && *m >= 0 && *m <= 59) {
            out_h = move(h);
            out_m = move(m);
            return true;
        }
        return false;
    };
    auto lex_seconds = [&]() { return lex_n_digits(2, seconds) && *seconds >= 0 && *seconds <= 59; };
    auto lex_milliseconds = [&]() { return lex_n_digits(3, milliseconds); };
    auto lex_seconds_milliseconds = [&]() { return lex_seconds() && (!lexer.consume_specific('.') || lex_milliseconds()); };
    auto lex_timezone = [&]() {
        if (lexer.consume_specific('+')) {
            timezone = '+';
            return lex_hours_minutes(timezone_hours, timezone_minutes);
        }
        if (lexer.consume_specific('-')) {
            timezone = '-';
            return lex_hours_minutes(timezone_hours, timezone_minutes);
        }
        if (lexer.consume_specific('Z'))
            timezone = 'Z';
        return true;
    };
    auto lex_time = [&]() { return lex_hours_minutes(hours, minutes) && (!lexer.consume_specific(':') || lex_seconds_milliseconds()) && lex_timezone(); };

    if (!lex_date() || (lexer.consume_specific('T') && !lex_time()) || !lexer.is_eof()) {
        return js_nan();
    }

    VERIFY(year.has_value()); 
    struct tm tm = {};
    tm.tm_year = *year - 1900;
    tm.tm_mon = !month.has_value() ? 0 : *month - 1;
    tm.tm_mday = day.value_or(1);
    tm.tm_hour = hours.value_or(0);
    tm.tm_min = minutes.value_or(0);
    tm.tm_sec = seconds.value_or(0);

    time_t timestamp;
    if (timezone.has_value() || !hours.has_value())
        timestamp = timegm(&tm);
    else
        timestamp = mktime(&tm);

    if (timezone == '-')
        timestamp += (*timezone_hours * 60 + *timezone_minutes) * 60;
    else if (timezone == '+')
        timestamp -= (*timezone_hours * 60 + *timezone_minutes) * 60;


    return Value(1000.0 * timestamp + milliseconds.value_or(0));
}

DateConstructor::DateConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Date.as_string(), *global_object.function_prototype())
{
}

void DateConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.date_prototype(), 0);

    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.now, now, 0, attr);
    define_native_function(vm.names.parse, parse, 1, attr);
    define_native_function(vm.names.UTC, utc, 1, attr);

    define_direct_property(vm.names.length, Value(7), Attribute::Configurable);
}

DateConstructor::~DateConstructor()
{
}

struct DatetimeAndMilliseconds {
    Core::DateTime datetime;
    i16 milliseconds { 0 };
};

static DatetimeAndMilliseconds now()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    auto datetime = Core::DateTime::now();
    auto milliseconds = static_cast<i16>(tv.tv_usec / 1000);
    return { datetime, milliseconds };
}

Value DateConstructor::call()
{
    auto [datetime, milliseconds] = JS::now();
    auto* date = Date::create(global_object(), datetime, milliseconds);
    return js_string(heap(), date->string());
}

Value DateConstructor::construct(FunctionObject& new_target)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    if (vm.argument_count() == 0) {
        auto [datetime, milliseconds] = JS::now();
        return ordinary_create_from_constructor<Date>(global_object, new_target, &GlobalObject::date_prototype, datetime, milliseconds, false);
    }

    auto create_invalid_date = [&global_object, &new_target]() {
        auto datetime = Core::DateTime::create(1970, 1, 1, 0, 0, 0);
        auto milliseconds = static_cast<i16>(0);
        return ordinary_create_from_constructor<Date>(global_object, new_target, &GlobalObject::date_prototype, datetime, milliseconds, true);
    };

    if (vm.argument_count() == 1) {
        auto value = vm.argument(0);
        if (value.is_string())
            value = parse_simplified_iso8601(value.as_string().string());
        else
            value = value.to_number(global_object);

        if (vm.exception())
            return {};

        if (!value.is_finite_number()) {
            return create_invalid_date();
        }

        double value_as_double = value.as_double();
        if (value_as_double > Date::time_clip)
            return create_invalid_date();
        auto datetime = Core::DateTime::from_timestamp(static_cast<time_t>(value_as_double / 1000));
        auto milliseconds = static_cast<i16>(fmod(value_as_double, 1000));
        return ordinary_create_from_constructor<Date>(global_object, new_target, &GlobalObject::date_prototype, datetime, milliseconds, false);
    }

    auto arg_or = [&vm, &global_object](size_t i, i32 fallback) {
        return vm.argument_count() > i ? vm.argument(i).to_number(global_object) : Value(fallback);
    };

    auto year_value = vm.argument(0).to_number(global_object);
    if (vm.exception())
        return {};
    if (!year_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto year = year_value.as_i32();

    auto month_index_value = vm.argument(1).to_number(global_object);
    if (vm.exception())
        return {};
    if (!month_index_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto month_index = month_index_value.as_i32();

    auto day_value = arg_or(2, 1);
    if (vm.exception())
        return {};
    if (!day_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto day = day_value.as_i32();

    auto hours_value = arg_or(3, 0);
    if (vm.exception())
        return {};
    if (!hours_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto hours = hours_value.as_i32();

    auto minutes_value = arg_or(4, 0);
    if (vm.exception())
        return {};
    if (!minutes_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto minutes = minutes_value.as_i32();

    auto seconds_value = arg_or(5, 0);
    if (vm.exception())
        return {};
    if (!seconds_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto seconds = seconds_value.as_i32();

    auto milliseconds_value = arg_or(6, 0);
    if (vm.exception())
        return {};
    if (!milliseconds_value.is_finite_number()) {
        return create_invalid_date();
    }
    auto milliseconds = milliseconds_value.as_i32();

    seconds += milliseconds / 1000;
    milliseconds %= 1000;
    if (milliseconds < 0) {
        seconds -= 1;
        milliseconds += 1000;
    }

    if (year >= 0 && year <= 99)
        year += 1900;
    int month = month_index + 1;
    auto datetime = Core::DateTime::create(year, month, day, hours, minutes, seconds);
    auto time = datetime.timestamp() * 1000.0 + milliseconds;
    if (time > Date::time_clip)
        return create_invalid_date();
    return ordinary_create_from_constructor<Date>(global_object, new_target, &GlobalObject::date_prototype, datetime, milliseconds, false);
}

JS_DEFINE_NATIVE_FUNCTION(DateConstructor::now)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return Value(floor(tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0));
}

JS_DEFINE_NATIVE_FUNCTION(DateConstructor::parse)
{
    if (!vm.argument_count())
        return js_nan();

    auto iso_8601 = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};

    return parse_simplified_iso8601(iso_8601);
}

JS_DEFINE_NATIVE_FUNCTION(DateConstructor::utc)
{
    auto arg_or = [&vm, &global_object](size_t i, i32 fallback) { return vm.argument_count() > i ? vm.argument(i).to_i32(global_object) : fallback; };
    int year = vm.argument(0).to_i32(global_object);
    if (year >= 0 && year <= 99)
        year += 1900;

    struct tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = arg_or(1, 0); 
    tm.tm_mday = arg_or(2, 1);
    tm.tm_hour = arg_or(3, 0);
    tm.tm_min = arg_or(4, 0);
    tm.tm_sec = arg_or(5, 0);


    int milliseconds = arg_or(6, 0);
    return Value(1000.0 * timegm(&tm) + milliseconds);
}

}
