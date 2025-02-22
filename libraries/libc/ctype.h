/**
 * @file ctype.h
 * @author Krisna Pranav
 * @brief CTYPE
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

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

extern char const _ctype_[256];

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isalnum(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U | _L | _N);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isalpha(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U | _L);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isascii(int c)
{
    return (unsigned)c <= 127;
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_iscntrl(int c)
{
    return _ctype_[(unsigned char)(c)] & (_C);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isdigit(int c)
{
    return _ctype_[(unsigned char)(c)] & (_N);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isxdigit(int c)
{
    return _ctype_[(unsigned char)(c)] & (_N | _X);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isspace(int c)
{
    return _ctype_[(unsigned char)(c)] & (_S);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_ispunct(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isprint(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P | _U | _L | _N | _B);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isgraph(int c)
{
    return _ctype_[(unsigned char)(c)] & (_P | _U | _L | _N);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_islower(int c)
{
    return _ctype_[(unsigned char)(c)] & (_L);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isupper(int c)
{
    return _ctype_[(unsigned char)(c)] & (_U);
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_isblank(int c)
{
    return _ctype_[(unsigned char)(c)] & (_B) || (c == '\t');
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_toascii(int c)
{
    return c & 127;
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c | 0x20;

    return c;
}

/**
 * @param c 
 * @return int 
 */
static inline int __inline_toupper(int c)
{
    if (c >= 'a' && c <= 'z')
        return c & ~0x20;
        
    return c;
}

#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int c);
int isalpha(int c);
int isascii(int c);
int iscntrl(int c);
int isdigit(int c);
int isxdigit(int c);
int isspace(int c);
int ispunct(int c);
int isprint(int c);
int isgraph(int c);
int islower(int c);
int isupper(int c);
int isblank(int c);
int toascii(int c);
int tolower(int c);
int toupper(int c);

#ifdef __cplusplus
}
#endif
#define isalnum __inline_isalnum
#define isalpha __inline_isalpha
#define isascii __inline_isascii
#define iscntrl __inline_iscntrl
#define isdigit __inline_isdigit
#define isxdigit __inline_isxdigit
#define isspace __inline_isspace
#define ispunct __inline_ispunct
#define isprint __inline_isprint
#define isgraph __inline_isgraph
#define islower __inline_islower
#define isupper __inline_isupper
#define isblank __inline_isblank
#define toascii __inline_toascii
#define tolower __inline_tolower
#define toupper __inline_toupper

__END_DECLS
