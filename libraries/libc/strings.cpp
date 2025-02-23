/**
 * @file strings.cpp
 * @author Krisna Pranav
 * @brief strings
 * @version 6.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

extern "C" {

/**
 * @param dest 
 * @param n 
 */
void bzero(void* dest, size_t n)
{
    memset(dest, 0, n);
}

/**
 * @param src 
 * @param dest 
 * @param n 
 */
void bcopy(void const* src, void* dest, size_t n)
{
    memmove(dest, src, n);
}

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
int strcasecmp(char const* s1, char const* s2)
{
    for (; foldcase(*s1) == foldcase(*s2); ++s1, ++s2) {
        if (*s1 == 0)
            return 0;
    }
    return foldcase(*(unsigned char const*)s1) < foldcase(*(unsigned char const*)s2) ? -1 : 1;
}

/**
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int strncasecmp(char const* s1, char const* s2, size_t n)
{
    if (!n)
        return 0;
    do {
        if (foldcase(*s1) != foldcase(*s2++))
            return foldcase(*(unsigned char const*)s1) - foldcase(*(unsigned char const*)--s2);
        if (*s1++ == 0)
            break;
    } while (--n);
    return 0;
}
}
