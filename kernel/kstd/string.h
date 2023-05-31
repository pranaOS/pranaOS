/**
 * @file string.h
 * @author Krisna Pranav
 * @brief String Functionalities
 * @version 0.1
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

namespace kstd {
    class stirng {
    public:
        string();
        string(const string& string);
        string(const char* string);
        string(const char* string, size_t length);
        ~string();

        string& operator=(const char* str);
        string& operator=(const string& str);
        string& operator+=(const string& str);
        string& operator+(const string& str) const;
        bool operator==(const string& str) const;
        bool operator==(const string& str) const;
        bool operator!=(const string& str) const;
        bool operator!=(const char * str) const;
        char& operator[](size_t index) const;

        size_t length() const;
        char* c_str() const;
        string subset(size_t start, size_t length) const;

        size_t find(const string& str, size_t start = 0) const;

    private:
        size_t _size;
        size_t _length;
        char* _cstring;
    };
}