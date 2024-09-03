/**
 * @file string.h
 * @author Krisna Pranav
 * @brief serial
 * @version 6.0
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <string>
#include <sstream>
#include <ArduinoJson.hpp>
#include "defines.h"

class StringSumHelper;

class String : public std::string
{
  public:
    String() : std::string() {};
    String(const char* str) : std::string(str) {};

#define _FAKE_STR_CONSTR(T) String(T smth) : std::string(std::to_string(smth)) { };

    _FAKE_STR_CONSTR(bool)
    _FAKE_STR_CONSTR(char)

#define _FAKE_STR_CONSTR_BASED(T) String(T smth, int base = 10) : std::string(String::numberToString(smth, base)) { };
    _FAKE_STR_CONSTR_BASED(float)
    _FAKE_STR_CONSTR_BASED(double)
    _FAKE_STR_CONSTR_BASED(unsigned char)
    _FAKE_STR_CONSTR_BASED(short)
    _FAKE_STR_CONSTR_BASED(unsigned short)
    _FAKE_STR_CONSTR_BASED(int)
    _FAKE_STR_CONSTR_BASED(unsigned int)
    _FAKE_STR_CONSTR_BASED(long)
    _FAKE_STR_CONSTR_BASED(unsigned long)

#define _FAKE_STR_CPY_CONSTR(T) String(T smth) : std::string(smth) {};
    _FAKE_STR_CPY_CONSTR(std::string)

    ~String() {};

    size_t write(uint8_t c)
    {
        this->append(std::to_string(c));
    } // size_t write

    size_t write(const uint8_t* s, size_t n)
    {
        for (size_t i = 0; i < n; i++)
            this->append(std::to_string(s[n]));
        return this->size();
    } // size_t write

    int toInt() const
    {
        return strtol(this->c_str(), nullptr, 10);
    }

    float toFloat() const
    {
        return strtof(this->c_str(), nullptr);
    }

    double toDouble() const
    {
        return strtod(this->c_str(), nullptr);
    }

    String substring(int from, int to = -1) const
    {
        if (to == -1)
            return String(this->substr(from));
        else
            return String(this->substr(from, to - from));
    }
}; // class STring : public std::string
