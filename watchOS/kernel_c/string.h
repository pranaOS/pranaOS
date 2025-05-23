/**
 * @file string.h
 * @author Krisna Pranav
 * @brief serial
 * @version 6.0
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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

    /**
     * @param from 
     * @param to 
     * @return String 
     */
    String substring(int from, int to = -1) const
    {
        if (to == -1)
            return String(this->substr(from));
        else
            return String(this->substr(from, to - from));
    }

    /**
     * @tparam T 
     * @param smth 
     * @return String 
     */
    template<typename T> String operator+(const T& smth) const 
    {
        String res(*this);

        if constexpr (std::is_same<T, short>::value or std::is_same<T, int>::value or std::is_same<T, long>::value or std::is_same<T, unsigned short>::value or std::is_same<T, unsigned int>::value or std::is_same<T, unsigned long>::value or std::is_same<T, float>::value or std::is_same<T, double>::value) {
            std::stringstream sstream;
            sstream << res << smth; 
            res = sstream.str();
        } else if constexpr (std::is_same<T, char>::value or std::is_same<T, unsigned char>::value)
            res.push_back(smth); 
        else
            res.append(smth);
        return res;
    }

    /**
     * @tparam T 
     * @param smth 
     * @return String& 
     */
    template<typename T> String& operator+=(const T& smth) 
    {
        if constexpr (std::is_same<T, float>::value or std::is_same<T, double>::value or std::is_same<T, short>::value or std::is_same<T, int>::value or std::is_same<T, long>::value or std::is_same<T, unsigned short>::value or std::is_same<T, unsigned int>::value or std::is_same<T, unsigned long>::value) {
            std::stringstream sstream;
            sstream << *this << smth; 
            *this = sstream.str();
        } else if constexpr (std::is_same<T, char>::value or std::is_same<T, unsigned char>::value)
            this->push_back(smth); 
        else
            this->append(smth);
        return *this;
    }

    friend class StringSumHelper;

    /**
     * @param lhs 
     * @param rhs 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, const String& rhs);

    /**
     * @param lhs 
     * @param c 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, char c);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned char num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, short num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned short num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, int num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned int num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, long num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned long num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, float num);

    /**
     * @param lhs 
     * @param num 
     * @return StringSumHelper& 
     */
    friend StringSumHelper& operator + (const StringSumHelper& lhs, double num);

  private:
    /**
     * @param num 
     * @param base 
     * @return std::string 
     */
    static std::string numberToString(auto num, int base)
    {
        std::stringstream sstream;

        switch (base) {
        case HEX:
            sstream << std::hex << num;
            break;
        case DEC:
            sstream << std::dec << num;
            break;
        case OCT:
            sstream << std::oct << num;
            break;
        default:
            throw std::invalid_argument("Unknown base for numberToString");
        }
        return sstream.str();
    }
}; // class STring : public std::string


class StringSumHelper : public String {
  public:

    /**
     * @param c 
     */
    StringSumHelper(char c) : String(c) {}

    /**
     * @param num 
     */
    StringSumHelper(unsigned char num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(short num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(unsigned short num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(int num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(unsigned int num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(long num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(unsigned long num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(float num) : String(num) {}

    /**
     * @param num 
     */
    StringSumHelper(double num) : String(num) {}
}; // class StringSumHelper

/**
 * @param t 
 * @param variant 
 * @return true 
 * @return false 
 */
inline bool convertToJson(const String& t, ArduinoJson::JsonVariant variant) {
    return variant.set(t.c_str());
}