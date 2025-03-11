/**
 * @file timezone.h
 * @author Krisna Pranav
 * @brief Time Zone
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/error.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <mods/string_view.h>
#include <mods/time.h>
#include <mods/types.h>
#include <libtimezone/forward.h>

namespace TimeZone 
{
    enum class InDST 
    {
        No,
        Yes,
    }; // enum class InDST 
 
    struct Offset 
    {
        i64 seconds { 0 };
        InDST in_dst { InDST::No };
    }; // struct Offset 

    struct NamedOffset : public Offset 
    {
        String name;
    }; // struct NamedOffset : public Offset 

    struct Coordinate 
    {
        constexpr float decimal_coordinate() const
        {
            return static_cast<float>(degrees) + (static_cast<float>(minutes) / 60.0f) + (static_cast<float>(seconds) / 3'600.0f);
        }

        i16 degrees { 0 };
        u8 minutes { 0 };
        u8 seconds { 0 };
    }; // struct Coordinate 

    struct Location 
    {
        Coordinate latitude;
        Coordinate longitude;
    }; // struct Location 

    StringView system_time_zone();
    StringView current_time_zone();

    /**
     * @param time_zone 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> change_time_zone(StringView time_zone);

    Span<StringView const> all_time_zones();

    /**
     * @param time_zone 
     * @return Optional<TimeZone> 
     */
    Optional<TimeZone> time_zone_from_string(StringView time_zone);

    /**
     * @param time_zone 
     * @return StringView 
     */
    StringView time_zone_to_string(TimeZone time_zone);

    /**
     * @param time_zone 
     * @return Optional<StringView> 
     */
    Optional<StringView> canonicalize_time_zone(StringView time_zone);

    /**
     * @param daylight_savings_rule 
     * @return Optional<DaylightSavingsRule> 
     */
    Optional<DaylightSavingsRule> daylight_savings_rule_from_string(StringView daylight_savings_rule);

    /**
     * @param daylight_savings_rule 
     * @return StringView 
     */
    StringView daylight_savings_rule_to_string(DaylightSavingsRule daylight_savings_rule);

    /**
     * @brief Get the time zone offset object
     * 
     * @param time_zone 
     * @param time 
     * @return Optional<Offset> 
     */
    Optional<Offset> get_time_zone_offset(TimeZone time_zone, Mods::Time time);

    /**
     * @brief Get the time zone offset object
     * 
     * @param time_zone 
     * @param time 
     * @return Optional<Offset> 
     */
    Optional<Offset> get_time_zone_offset(StringView time_zone, Mods::Time time);

    /**
     * @brief Get the named time zone offsets object
     * 
     * @param time_zone 
     * @param time 
     * @return Optional<Array<NamedOffset, 2>> 
     */
    Optional<Array<NamedOffset, 2>> get_named_time_zone_offsets(TimeZone time_zone, Mods::Time time);

    /**
     * @brief Get the named time zone offsets object
     * 
     * @param time_zone 
     * @param time 
     * @return Optional<Array<NamedOffset, 2>> 
     */
    Optional<Array<NamedOffset, 2>> get_named_time_zone_offsets(StringView time_zone, Mods::Time time);

    /**
     * @brief Get the time zone location object
     * 
     * @param time_zone 
     * @return Optional<Location> 
     */
    Optional<Location> get_time_zone_location(TimeZone time_zone);
    
    /**
     * @brief Get the time zone location object
     * 
     * @param time_zone 
     * @return Optional<Location> 
     */
    Optional<Location> get_time_zone_location(StringView time_zone); 
} // namespace TimeZone
