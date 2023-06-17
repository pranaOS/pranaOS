/**
 * @file temperature.hpp
 * @author Krisna Pranav
 * @brief Temperature Checker & Functionalit
 * @version 1.0
 * @date 2023-06-16
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <cstdint>
#include <iomanip>
#include <optional>
#include <string>

namespace Utils::temperature {

    /**
     * @brief Symbols, Degree, DegreeSymbol
     * 
     */
    inline constexpr auto celsiusSymbol          = "C";
    inline constexpr auto fahrenheitSymbol       = "F";
    inline constexpr auto degree                 = "\u00B0";
    inline constexpr auto celsiusDegreeSymbol    = "\u00B0C";
    inline constexpr auto fahrenheitDegreeSymbol = "\u00B0F";

    /**
     * @brief Temperature
     * 
     */
    struct Temperature {
        using Value = float;
        enum class Unit {
            Celsius,
            Fahrenheit
        };

        Unit unit;
        Value value;
    };

    /**
     * @brief celsiusToFahrenheit
     * 
     * @param value 
     * @return float 
     */
    inline float celsiusToFahrenheit(float value) {
        return (value * 1.8) + 32;
    }

    /**
     * @brief unitToStr
     * 
     * @param unit 
     * @return std::string 
     */
    inline std::string unitToStr(Temperature::Unit unit) {
        return unit == Temperature::Unit::Celsius ? celsiusSymbol : fahrenheitSymbol;
    }

    /**
     * @brief strToUnit
     * 
     * @param str 
     * @return std::optional<Temperature::Unit> 
     */
    inline std::optional<Temperature::Unit> strToUnit(std::string_view str) {
        if ((str == celsiusDegreeSymbol) || (str == celsiusSymbol)) {
            return Temperature::Unit::Celsius;
        }
        else if ((str == fahrenheitDegreeSymbol) || (str == fahrenheitSymbol)) {
            return Temperature::Unit::Fahrenheit;
        }
        return {};
    }

    /**
     * @brief tempToStrFloat
     * 
     * @param temperature 
     * @param precision 
     * @return std::string 
     */
    inline std::string tempToStrFloat(Temperature temperature, const int precision = 1) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision);
        stream << temperature.value << degree << utils::temperature::unitToStr(temperature.unit);
        return stream.str();
    }

    /**
     * @brief tempToStrDec
     * 
     * @param temperature 
     * @return std::string 
     */
    inline std::string tempToStrDec(Temperature temperature) {
        return std::to_string(static_cast<std::int32_t>(temperature.value)) + degree +
               utils::temperature::unitToStr(temperature.unit);
    }
} 