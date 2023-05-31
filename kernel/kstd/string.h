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

#include "kstd/types.h"

namespace kstd {
	class string {
	public:
        /**
         * @brief Construct a new string object
         * 
         */
		string();
		string(const string& string);
		string(const char* string);
		string(const char* string, size_t length);
		~string();

        /**
         * @brief opeartors
         * 
         * @param str 
         * @return string& 
         */
		string& operator=(const char* str);
		string& operator=(const string& str);
		string& operator+=(const string& str);
		string operator+(const string& b) const;
		bool operator==(const string& str) const;
		bool operator==(const char* str) const;
		bool operator!=(const string& str) const;
		bool operator!=(const char* str) const;
		char& operator[](size_t index) const;

        /**
         * @brief length
         * 
         * @return size_t 
         */
		size_t length() const;

        /**
         * @brief c_str
         * 
         * @return char* 
         */
		char* c_str() const;

        /**
         * @brief data
         * 
         * @return char* 
         */
		char* data() const;

        /**
         * @brief substr
         * 
         * @param start 
         * @param length 
         * @return string 
         */
		string substr(size_t start, size_t length) const;

        /**
         * @brief find
         * 
         * @param str 
         * @param start 
         * @return size_t 
         */
		size_t find(const string& str, size_t start = 0) const;
		size_t find(const char* str, size_t start = 0) const;
		size_t find(const char c, size_t start = 0) const;

        /**
         * @brief find_last_of
         * 
         * @param str 
         * @param end 
         * @return size_t 
         */
		size_t find_last_of(const string& str, size_t end = -1) const;
		size_t find_last_of(const char* str, size_t end = -1) const;
		size_t find_last_of(const char c, size_t end = -1) const;
        
	private:
		size_t _size;
		size_t _length;
		char* _cstring;
	};
}
