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
    /**
     * @brief Construct a new Field object
     * 
     */
    Field() : mValue(nullptr)
    {}

    /**
     * @brief Construct a new Field object
     * 
     * @param value 
     */
    Field(const char *value) {
        if (value != nullptr) {
            mValue = value;
        }
    }

    /**
     * @brief Destroy the Field object
     * 
     */
    ~Field()
    {}

    /**
     * @brief getCString
     * 
     * @return const char* 
     */
    const char *getCString() const;

    /**
     * @brief Get the String object
     * 
     * @return const std::string& 
     */
    const std::string &getString() const;
    
    /**
     * @brief Get the Float object
     * 
     * @return float 
     */
    float getFloat() const;

    /**
     * @brief Get the Bool object
     * 
     * @return true 
     * @return false 
     */
    bool getBool() const;

    /**
     * @brief Get the Double object
     * 
     * @return double 
     */
    double getDouble() const;

    /**
     * @brief Get the Int8 object
     * 
     * @return std::int8_t 
     */
    std::int8_t getInt8() const;

    /**
     * @brief Get the Int32 object
     * 
     * @return std::int32_t 
     */
    std::int32_t getInt32() const;

    /**
     * @brief getUInt8
     * 
     * @return std::uint8_t 
     */
    std::uint8_t getUInt8() const;

    /**
     * @brief getUInt16
     * 
     * @return std::uint16_t 
     */
    std::uint16_t getUInt16() const;

    /**
     * @brief Get the Int16 object
     * 
     * @return std::int16_t 
     */
    std::int16_t getInt16() const;

    /**
     * @brief getUInt32
     * 
     * @return std::uint32_t 
     */
    std::uint32_t getUInt32() const;

    /**
     * @brief getUInt64
     * 
     * @return std::uint64_t 
     */
    std::uint64_t getUInt64() const;

    /**
     * @brief Get the Int64 object
     * 
     * @return std::int64_t 
     */
    std::int64_t getInt64() const;

    /**
     * @brief Set the Value object
     * 
     * @param value 
     */
    void setValue(const char *value);

private:
    /**
     * @brief value
     * 
     */
    std::string mValue;
};