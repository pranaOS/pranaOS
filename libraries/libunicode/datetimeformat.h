/**
 * @file datetimeformat.h
 * @author Krisna Pranav
 * @brief Date Time Format
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/time.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <libtimezone/timezone.h>
#include <libunicode/forward.h>

namespace Unicode 
{
    enum class Era : u8 
    {
        BC,
        AD,
    }; // enum class Era : u8 

    enum class Month : u8 
    {
        January,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        October,
        November,
        December,
    }; // enum class Month : u8 

    enum class Weekday : u8 
    {
        Sunday,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
    }; // enum class Weekday : u8 

    enum class DayPeriod : u8 
    {
        AM,
        PM,
        Morning1,
        Morning2,
        Afternoon1,
        Afternoon2,
        Evening1,
        Evening2,
        Night1,
        Night2,
    }; // enum class DayPeriod : u8 

    enum class HourCycle : u8 
    {
        H11,
        H12,
        H23,
        H24,
    }; // enum class HourCycle : u8 

    enum class CalendarPatternStyle : u8 
    {
        Narrow,
        Short,
        Long,
        Numeric,
        TwoDigit,
        ShortOffset,
        LongOffset,
        ShortGeneric,
        LongGeneric,
    }; // enum class CalendarPatternStyle : u8 
 
    struct CalendarPattern 
    {
        enum class Field 
        {
            Era,
            Year,
            Month,
            Weekday,
            Day,
            DayPeriod,
            Hour,
            Minute,
            Second,
            FractionalSecondDigits,
            TimeZoneName,
        }; // enum class Field 

        /**
         * @tparam Callback 
         * @param other 
         * @param callback 
         */
        template<typename Callback>
        void for_each_calendar_field_zipped_with(CalendarPattern const& other, Callback&& callback)
        {
            callback(era, other.era, Field::Era);
            callback(year, other.year, Field::Year);
            callback(month, other.month, Field::Month);
            callback(weekday, other.weekday, Field::Weekday);
            callback(day, other.day, Field::Day);
            callback(day_period, other.day_period, Field::DayPeriod);
            callback(hour, other.hour, Field::Hour);
            callback(minute, other.minute, Field::Minute);
            callback(second, other.second, Field::Second);
            callback(fractional_second_digits, other.fractional_second_digits, Field::FractionalSecondDigits);
            callback(time_zone_name, other.time_zone_name, Field::TimeZoneName);
        }

        String skeleton {};
        String pattern {};
        Optional<String> pattern12 {};
        Optional<HourCycle> hour_cycle {};

        Optional<CalendarPatternStyle> era {};
        Optional<CalendarPatternStyle> year {};
        Optional<CalendarPatternStyle> month {};
        Optional<CalendarPatternStyle> weekday {};
        Optional<CalendarPatternStyle> day {};
        Optional<CalendarPatternStyle> day_period {};
        Optional<CalendarPatternStyle> hour {};
        Optional<CalendarPatternStyle> minute {};
        Optional<CalendarPatternStyle> second {};
        Optional<u8> fractional_second_digits {};
        Optional<CalendarPatternStyle> time_zone_name {};
    }; // struct CalendarPattern

    struct CalendarRangePattern : public CalendarPattern 
    {
        enum class Field 
        {
            Era,
            Year,
            Month,
            Day,
            AmPm,
            DayPeriod,
            Hour,
            Minute,
            Second,
            FractionalSecondDigits,
        }; // enum class Field 

        Optional<Field> field {};
        String start_range {};
        StringView separator {};
        String end_range {};
    }; // struct CalendarRangePattern : public CalendarPattern 

    enum class CalendarFormatType : u8 
    {
        Date,
        Time,
        DateTime,
    }; // enum class CalendarFormatType : u8 

    struct CalendarFormat 
    {
        CalendarPattern full_format {};
        CalendarPattern long_format {};
        CalendarPattern medium_format {};
        CalendarPattern short_format {};
    }; // struct CalendarFormat 

    enum class CalendarSymbol : u8 
    {
        DayPeriod,
        Era,
        Month,
        Weekday,
    }; // enum class CalendarSymbol : u8 

    struct TimeZoneFormat 
    {
        StringView symbol_ahead_sign {};
        StringView symbol_ahead_separator {};

        StringView symbol_behind_sign {};
        StringView symbol_behind_separator {};

        StringView gmt_format {};
        StringView gmt_zero_format {};
    }; // struct TimeZoneFormat 
    
    /**
     * @param hour_cycle 
     * @return HourCycle 
     */
    HourCycle hour_cycle_from_string(StringView hour_cycle);

    /**
     * @param hour_cycle 
     * @return StringView 
     */
    StringView hour_cycle_to_string(HourCycle hour_cycle);

    /**
     * @param style 
     * @return CalendarPatternStyle 
     */
    CalendarPatternStyle calendar_pattern_style_from_string(StringView style);

    /**
     * @param style 
     * @return StringView 
     */
    StringView calendar_pattern_style_to_string(CalendarPatternStyle style);

    /**
     * @param hour_cycle_region 
     * @return Optional<HourCycleRegion> 
     */
    Optional<HourCycleRegion> hour_cycle_region_from_string(StringView hour_cycle_region);

    /**
     * @brief Get the regional hour cycles object
     * 
     * @param region 
     * @return Vector<HourCycle> 
     */
    Vector<HourCycle> get_regional_hour_cycles(StringView region);

    /**
     * @brief Get the locale hour cycles object
     * 
     * @param locale 
     * @return Vector<Unicode::HourCycle> 
     */
    Vector<Unicode::HourCycle> get_locale_hour_cycles(StringView locale);

    /**
     * @brief Get the default regional hour cycle object
     * 
     * @param locale 
     * @return Optional<Unicode::HourCycle> 
     */
    Optional<Unicode::HourCycle> get_default_regional_hour_cycle(StringView locale);

    /**
     * @param first 
     * @param second 
     * @return String 
     */
    String combine_skeletons(StringView first, StringView second);

    /**
     * @brief Get the calendar date format object
     * 
     * @param locale 
     * @param calendar 
     * @return Optional<CalendarFormat> 
     */
    Optional<CalendarFormat> get_calendar_date_format(StringView locale, StringView calendar);

    /**
     * @brief Get the calendar time format object
     * 
     * @param locale 
     * @param calendar 
     * @return Optional<CalendarFormat> 
     */
    Optional<CalendarFormat> get_calendar_time_format(StringView locale, StringView calendar);

    /**
     * @brief Get the calendar date time format object
     * 
     * @param locale 
     * @param calendar 
     * @return Optional<CalendarFormat> 
     */
    Optional<CalendarFormat> get_calendar_date_time_format(StringView locale, StringView calendar);

    /**
     * @brief Get the calendar format object
     * 
     * @param locale 
     * @param calendar 
     * @param type 
     * @return Optional<CalendarFormat> 
     */
    Optional<CalendarFormat> get_calendar_format(StringView locale, StringView calendar, CalendarFormatType type);

    /**
     * @brief Get the calendar available formats object
     * 
     * @param locale 
     * @param calendar 
     * @return Vector<CalendarPattern> 
     */
    Vector<CalendarPattern> get_calendar_available_formats(StringView locale, StringView calendar);

    /**
     * @brief Get the calendar default range format object
     * 
     * @param locale 
     * @param calendar 
     * @return Optional<Unicode::CalendarRangePattern> 
     */
    Optional<Unicode::CalendarRangePattern> get_calendar_default_range_format(StringView locale, StringView calendar);

    /**
     * @brief Get the calendar range formats object
     * 
     * @param locale 
     * @param calendar 
     * @param skeleton 
     * @return Vector<Unicode::CalendarRangePattern> 
     */
    Vector<Unicode::CalendarRangePattern> get_calendar_range_formats(StringView locale, StringView calendar, StringView skeleton);

    /**
     * @brief Get the calendar range12 formats object
     * 
     * @param locale 
     * @param calendar 
     * @param skeleton 
     * @return Vector<Unicode::CalendarRangePattern> 
     */
    Vector<Unicode::CalendarRangePattern> get_calendar_range12_formats(StringView locale, StringView calendar, StringView skeleton);

    /**
     * @brief Get the calendar era symbol object
     * 
     * @param locale 
     * @param calendar 
     * @param style 
     * @param value 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_calendar_era_symbol(StringView locale, StringView calendar, CalendarPatternStyle style, Unicode::Era value);

    /**
     * @brief Get the calendar month symbol object
     * 
     * @param locale 
     * @param calendar 
     * @param style 
     * @param value 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_calendar_month_symbol(StringView locale, StringView calendar, CalendarPatternStyle style, Unicode::Month value);

    /**
     * @brief Get the calendar weekday symbol object
     * 
     * @param locale 
     * @param calendar 
     * @param style 
     * @param value 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_calendar_weekday_symbol(StringView locale, StringView calendar, CalendarPatternStyle style, Unicode::Weekday value);

    /**
     * @brief Get the calendar day period symbol object
     * 
     * @param locale 
     * @param calendar 
     * @param style 
     * @param value 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_calendar_day_period_symbol(StringView locale, StringView calendar, CalendarPatternStyle style, Unicode::DayPeriod value);

    /**
     * @brief Get the calendar day period symbol for hour object
     * 
     * @param locale 
     * @param calendar 
     * @param style 
     * @param hour 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_calendar_day_period_symbol_for_hour(StringView locale, StringView calendar, CalendarPatternStyle style, u8 hour);

    /**
     * @param locale 
     * @param time_zone 
     * @param style 
     * @param time 
     * @return String 
     */
    String format_time_zone(StringView locale, StringView time_zone, CalendarPatternStyle style, Mods::Time time);

    /**
     * @brief Get the time zone name object
     * 
     * @param locale 
     * @param time_zone 
     * @param style 
     * @param in_dst 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_time_zone_name(StringView locale, StringView time_zone, CalendarPatternStyle style, TimeZone::InDST in_dst);

    /**
     * @brief Get the time zone format object
     * 
     * @param locale 
     * @return Optional<TimeZoneFormat> 
     */
    Optional<TimeZoneFormat> get_time_zone_format(StringView locale);

} // namespace Unicode