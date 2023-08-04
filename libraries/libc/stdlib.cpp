/**
 * @file stdlib.cpp
 * @author Krisna Pranav
 * @brief stdlib
 * @version 1.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */


#include <alloca.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mods/assertions.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <mods/utf8view.h>
#include <kernel/api/syscall.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * @param str 
 * @param endptr 
 */
static void strtons(const char* str, char** endptr)
{
    assert(endptr);
    char* ptr = const_cast<char*>(str);

    while (isspace(*ptr)) {
        ptr += 1;
    }

    *endptr = ptr;
}

enum Sign
{
    Negative,
    Positive,
}; 

/**
 * @param str 
 * @param endptr 
 * @return Sign 
 */
static Sign strtosign(const char* str, char** endptr)
{
    assert(endptr);

    if (*str == '+') {
        *endptr = const_cast<char*>(str + 1);
        return Sign::Positive;
    } else if (*str == '-') {
        *endptr = const_cast<char*>(str + 1);
        return Sign::Negative;
    } else {
        *endptr = const_cast<char*>(str);
        return Sign::Positive;
    }
}

enum DigitConsumeDecision
{
    Consumed,
    PosOverflow,
    NegOverflow,
    Invalid,
}; 

template<typename T, T min_value, T max_value>
class NumParser
{
    MOD_MAKE_NONMOVABLE(NumParser);

public:
    /**
     * @param sign 
     * @param base 
     */
    NumParser(Sign sign, int base)
        : m_base(base)
        , m_num(0)
        , m_sign(sign)
    {
        m_cutoff = positive() ? (max_value / base) : (min_value / base);
        m_max_digit_after_cutoff = positive() ? (max_value % base) : (min_value % base);
    }

    /**
     * @param ch 
     * @return int 
     */
    int parse_digit(char ch)
    {
        int digit;

        if (isdigit(ch))
            digit = ch - '0';
        else if (islower(ch))
            digit = ch - ('a' - 10);
        else if (isupper(ch))
            digit = ch - ('A' - 10);
        else
            return -1;

        if (static_cast<T>(digit) >= m_base)
            return -1;

        return digit;
    }

    /**
     * @param ch 
     * @return DigitConsumeDecision 
     */
    DigitConsumeDecision consume(char ch)
    {
        int digit = parse_digit(ch);
        
        if (digit == -1)
            return DigitConsumeDecision::Invalid;
        
        if (!can_append_digit(digit)) {
            if (m_sign != Sign::Negative) {
                return DigitConsumeDecision::PosOverflow;
            } else {
                return DigitConsumeDecision::NegOverflow;
            }
        }

        m_num *= m_base;
        m_num += positive() ? digit : -digit;

        return DigitConsumeDecision::Consumed;
    }

    /**
     * @return T 
     */
    T number() const
    {
        return m_num;
    }

private:
    /**
     * @param digit 
     * @return true 
     * @return false 
     */
    bool can_append_digit(int digit)
    {
        const bool is_below_cutoff = positive() ? (m_num < m_cutoff) : (m_num > m_cutoff);

        if (is_below_cutoff) {
            return true;
        } else {
            return m_num == m_cutoff && digit < m_max_digit_after_cutoff;
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool positive() const
    {
        return m_sign != Sign::Negative;
    }

    const T m_base;
    T m_num;
    T m_cutoff;
    int m_max_digit_after_cutoff;
    Sign m_sign;
}; // class NumParser

typedef NumParser<int, INT_MIN, INT_MAX> IntParser;

static bool is_either(char* str, int offset, char lower, char upper)
{
    char ch = *(str + offset);
    return ch == lower || ch == upper;
}

__attribute__((warn_unused_result)) int __generate_unique_filename(char* pattern)
{
    size_t length = strlen(pattern);

    if (length < 6 || memcmp(pattern + length - 6)) {
        errno = EINVAL;
        return -1;
    }

    size_t start = length - 6;

    static constexpr char random_characters[] = "abcdefghijklmnopqrstuvwxyz0123456789";

    for (int attempt = 0; attempt < 100; ++attempt) {
        for (int i = 0; i < 6; ++i)
            pattern[start + i] = random_characters
        
        struct stat st;
        int rc = lstat(pattern, &st);

        if (rc < 0 && errno == ENOENT)
            return 0;
    }

    errno = EEXIST;

    return -1;
}