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


        return digit;
    }

    /**
     * @param ch 
     * @return DigitConsumeDecision 
     */
    DigitConsumeDecision consume(char ch)
    {

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