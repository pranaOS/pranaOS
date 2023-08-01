/**
 * @file ctype.h
 * @author Krisna Pranav
 * @brief CTYPE
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

/// @brief: values
#define _U 01
#define _L 02
#define _N 04
#define _S 010
#define _P 020
#define _C 040
#define _X 0100
#define _B 0200

/// @brief ctype[256]
extern const char _ctype_[256];

/**
 * @return int 
 */
int tolower(int);

/**
 * @return int 
 */
int toupper(int);

/**
 * @param c 
 * @return int 
 */
static inline int isalnum(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_U | _L | _N));
}

/**
 * @param c 
 * @return int 
 */
static inline int isalpha(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_U | _L));
}

/**
 * @param c 
 * @return int 
 */
static inline int iscntrl(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_C));
}

/**
 * @param c 
 * @return int 
 */
static inline int isdigit(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_N));
}

/**
 * @param c 
 * @return int 
 */
static inline int isxdigit(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_N | _X));
}

/**
 * @param c 
 * @return int 
 */
static inline int isspace(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_S));
}

/**
 * @param c 
 * @return int 
 */
static inline int ispunct(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_P));
}

/**
 * @param c 
 * @return int 
 */
static inline int isprint(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_P | _U | _L | _N | _B));
}

/**
 * @param c 
 * @return int 
 */
static inline int isgraph(int c)
{
    return (_ctype_[(unsigned char)(c)] & (_P | _U | _L | _N));
}

/**
 * @param c 
 * @return int 
 */
static inline int islower(int c)
{
    return ((_ctype_[(unsigned char)(c)] & (_U | _L)) == _L);
}

/**
 * @param c 
 * @return int 
 */
static inline int isupper(int c)
{
    return ((_ctype_[(unsigned char)(c)] & (_U | _L)) == _U);
}

/// @brief: isascii
#define isascii(c) ((unsigned)c <= 127)

/// @brief: toascii
#define toascii(c) ((c)&127)

__END_DECLS