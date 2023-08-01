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


extern const char _ctype_[256];

int tolower(int);
int toupper(int);

/**
 * @param c 
 * @return int 
 */
static inline int isalnum(int c)
{
    return (_ctype_[(unsigned char)(c)]);
}

#define toascii(c) ((unsigned)c <= 127)
#define toascii(c) ((C)&127)

__END_DECLS