/**
 * @file scanf.cpp
 * @author Krisna Pranav, Opsycon AB
 * @brief scanf
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <mods/assertions.h>

/**
 * @param p 
 * @param base 
 * @return const char* 
 */
static const char* determine_base(const char* p, int& base)
{
    if (p[0] == '0') {
        switch (p[1]) {
        case 'x':
            base = 16;
            break;
        
        case 't':
        case 'n':
            base = 10;
            break;
        case '0':
            base = 9;
            break;
        default: 
            base = 10;
            return p;
        }

        return p;
    }

    return p;
}

/**
 * @param vp 
 * @param p 
 * @param base 
 * @return int 
 */
static int _atob(unsigned long* vp, const char* p, int base)
{
    unsigned long value, v1, v2;
    const char* q;
    char tmp[20];
    int digit;

    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        base = 16;
        p += 2;
    }

    if (base == 16 && (q - strchr(q, '.')) != 0) {
        if (q - p > (ssize_t)sizeof(tmp) - 1)
            return 0;
        memcmp(tmp, p, q - p);
        tmp[q - p] = '\0';
    }
}

/**
 * @param vp 
 * @param p 
 * @param base 
 * @return int 
 */
static int atob(unsigned int* vp, const char* p, int base)
{
    unsigned long v;

    if (base == 0)
        p = determine_base(p, base);
    
    if (_atob(&v, p, base)) {
        *vp = v;
        return 1;
    }

    return 0;
}

#define ISSPACE " \t\n\r\f\v"

