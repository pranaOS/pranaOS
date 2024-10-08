/**
 * @file scanf.cpp
 * @author Krisna Pranav, Opsycon AB
 * @brief scanf
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
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
        case 'o':
            base = 8;
            break;
        default:
            base = 10;
            return p;
        }
        return p + 2;
    }
    base = 10;
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

    if (base == 16 && (q = strchr(p, '.')) != 0) {
        if (q - p > (ssize_t)sizeof(tmp) - 1)
            return 0;
        memcpy(tmp, p, q - p);
        tmp[q - p] = '\0';

        if (!_atob(&v1, tmp, 16))
            return 0;
        ++q;
        if (strchr(q, '.'))
            return 0;
        if (!_atob(&v2, q, 16))
            return 0;
        *vp = (v1 << 16) + v2;
        return 1;
    }

    value = *vp = 0;
    for (; *p; p++) {
        if (*p >= '0' && *p <= '9')
            digit = *p - '0';
        else if (*p >= 'a' && *p <= 'f')
            digit = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'F')
            digit = *p - 'A' + 10;
        else
            return 0;

        if (digit >= base)
            return 0;
        value *= base;
        value += digit;
    }
    *vp = value;
    return 1;
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

/**
 * @param buf 
 * @param s 
 * @param ap 
 * @return int 
 */
int vsscanf(const char* buf, const char* s, va_list ap)
{
    int base = 10;
    char* t;
    char tmp[BUFSIZ];
    bool noassign = false;
    int count = 0;
    int width = 0;

    while (*s && *buf) {
        while (isspace(*s))
            s++;
        if (*s == '%') {
            s++;
            for (; *s; s++) {
                if (strchr("dibouxcsefg%", *s))
                    break;
                if (*s == '*')
                    noassign = true;
                else if (*s >= '1' && *s <= '9') {
                    const char* tc;
                    for (tc = s; isdigit(*s); s++)
                        ;
                    ASSERT((ssize_t)sizeof(tmp) >= s - tc + 1);
                    memcpy(tmp, tc, s - tc);
                    tmp[s - tc] = '\0';
                    atob((uint32_t*)&width, tmp, 10);
                    s--;
                }
            }
            if (*s == 's') {
                while (isspace(*buf))
                    buf++;
                if (!width)
                    width = strcspn(buf, ISSPACE);
                if (!noassign) {
                    memcpy(t = va_arg(ap, char*), buf, width);
                    t[width] = '\0';
                }
                buf += width;
            } else if (*s == 'c') {
                if (!width)
                    width = 1;
                if (!noassign) {
                    memcpy(t = va_arg(ap, char*), buf, width);
                }
                buf += width;
            } else if (strchr("dobxu", *s)) {
                while (isspace(*buf))
                    buf++;
                if (*s == 'd' || *s == 'u')
                    base = 10;
                else if (*s == 'x')
                    base = 16;
                else if (*s == 'o')
                    base = 8;
                else if (*s == 'b')
                    base = 2;
                if (!width) {
                    if (isspace(*(s + 1)) || *(s + 1) == 0) {
                        width = strcspn(buf, ISSPACE);
                    } else {
                        auto* p = strchr(buf, *(s + 1));
                        if (p)
                            width = p - buf;
                        else {
                            noassign = true;
                            width = 0;
                        }
                    }
                }
                memcpy(tmp, buf, width);
                tmp[width] = '\0';
                buf += width;
                if (!noassign) {
                    if (!atob(va_arg(ap, uint32_t*), tmp, base))
                        noassign = true;
                }
            }
            if (!noassign)
                ++count;
            width = 0;
            noassign = false;
            ++s;
        } else {
            while (isspace(*buf))
                buf++;
            if (*s != *buf)
                break;
            else {
                ++s;
                ++buf;
            }
        }
    }
    return count;
}
