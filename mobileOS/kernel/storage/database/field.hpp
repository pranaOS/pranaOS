/**
 * @file field.hpp
 * @author Krisna Pranav
 * @brief Fields
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdint.h>
#include <string>

class Field {
public:
    Field() : mValue(nullptr)
    {}

    Field(const char *value)
    {
        if (value != nullptr) {
            mValue = value;
        }
    }

    ~Field()
    {}

    const char *getCString() const;
    const std::string &getString() const;
    float getFloat() const;
    bool getBool() const;
    double getDouble() const;

    /**
     * @brief Get the Int8 object
     * 
     * @return std::int8_t 
     */
    std::int8_t getInt8() const;

    /**
     * @brief Set the Value objects
     * 
     * @param value 
     */
    void setValue(const char *value);

private:
    std::string mValue;
};