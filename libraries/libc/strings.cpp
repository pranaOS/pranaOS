/**
 * @file strings.cpp
 * @author Krisna Pranav
 * @brief strings
 * @version 6.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <strings.h>
#include <ctype.h>

extern "C" 
{

    /**
     * @param ch 
     * @return char 
     */
    static char foldcase(char ch)
    {

        if (isalpha(ch))
            return tolower(ch);

        return ch;
    }

    /**
     * @param s1 
     * @param s2 
     * @return int 
     */
    int strcasecmp(const char* s1, const char* s2)
    {
        for (; foldcase(*s1) == foldcase(*s2); ++s1, ++s2) {
            if (*s1 == 0)
                return 0;
        }

        return foldcase(*(const unsigned char*)s1) < foldcase(*(const unsigned char*)s2) ? -1 : 1;

    }

    /**
     * @param s1 
     * @param s2 
     * @param n 
     * @return int 
     */
    int strncasecmp(const char* s1, const char* s2, size_t n)
    {
        if (!n)
            return 0;

        do {
            if (foldcase(*s1) != foldcase(*s2++))
                return foldcase(*(const unsigned char*)s1) - foldcase(*(const unsigned char*)--s2);
            if (*s1++ == 0)
                break;
        } while (--n);

        return 0;
    }
    
} // extern