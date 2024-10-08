/**
 * @file ctype.cpp
 * @author Krisna Pranav
 * @brief ctype
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <ctype.h>

extern "C" 
{

    const char _ctype_[256] = {
        _C, _C, _C, _C, _C, _C, _C, _C,
        _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C,
        _C, _C, _C, _C, _C, _C, _C, _C,
        _C, _C, _C, _C, _C, _C, _C, _C,
        (char)(_S | _B), _P, _P, _P, _P, _P, _P, _P,
        _P, _P, _P, _P, _P, _P, _P, _P,
        _N, _N, _N, _N, _N, _N, _N, _N,
        _N, _N, _P, _P, _P, _P, _P, _P,
        _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,
        _U, _U, _U, _U, _U, _U, _U, _U,
        _U, _U, _U, _U, _U, _U, _U, _U,
        _U, _U, _U, _P, _P, _P, _P, _P,
        _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L,
        _L, _L, _L, _L, _L, _L, _L, _L,
        _L, _L, _L, _L, _L, _L, _L, _L,
        _L, _L, _L, _P, _P, _P, _P, _C
    };

    /**
     * @param c 
     * @return int 
     */
    int tolower(int c)
    {
        if (c >= 'A' && c <= 'Z')
            return c | 0x20;
        return c;
    }

    /**
     * @param c 
     * @return int 
     */
    int toupper(int c)
    {
        if (c >= 'a' && c <= 'z')
            return c & ~0x20;
        return c;
    }
    
}
