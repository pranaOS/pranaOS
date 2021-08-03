/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS


#define _U 01
#define _L 02
#define _N 04
#define _S 010
#define _P 020
#define _C 040
#define _X 0100
#define _B 0200

extern const char _ctype_[256];

static inline int __inline_isalnum(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U | _L | _N);
}

static inline int __inline_isalpha(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U | _L);
}

static inline int __inline_isascii(int c)
{
    return (unsigned)c <= 127;
}

static inline int __inline_iscntrl(int c)
{
    return _ctype_[(unsigned char)(c)] & (_C);
}

static inline int __inline_isdigit(int c)
{
    return _ctype_[(unsigned char)(c)] & (_N);
}

static inline int __inline_isxdigit(int c)
{
    return _ctype_[(unsigned char)(c)] & (_N | _X);
}

static inline int __inline_isspace(int c)
{
    return _ctype_[(unsigned char)(c)] & (_S);
}

static inline int __inline_ispunct(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P);
}

static inline int __inline_isprint(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P | _U | _L | _N | _B);
}

static inline int __inline_isgraph(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P | _U | _L | _N);
}

static inline int __inline_islower(int c)
{
    return _ctype_[(unsigned char)(c)] & (_L);
}

static inline int __inline_isupper(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U);
}

static inline int __inline_isblank(int c)
{
    return _ctype_[(unsigned char)(c)] & (_B) || (c == '\t');
}

static inline int __inline_toascii(int c)
{
    return c & 127;
}

static inline int __inline_tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c | 0x20;
    return c;
}

static inline int __inline_toupper(int c)
{
    if (c >= 'a' && c <= 'z')
        return c & ~0x20;
    return c;
}

__END_DECLS