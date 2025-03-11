/**
 * @file string.cpp
 * @author Krisna Pranav
 * @brief string
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <mods/memmem.h>
#include <mods/memory.h>
#include <mods/platform.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {

/**
 * @param s 
 * @param accept 
 * @return size_t 
 */
size_t strspn(char const* s, char const* accept)
{
    char const* p = s;
cont:
    char ch = *p++;
    char ac;
    for (char const* ap = accept; (ac = *ap++) != '\0';) {
        if (ac == ch)
            goto cont;
    }
    return p - 1 - s;
}

/**
 * @param s 
 * @param reject 
 * @return size_t 
 */
size_t strcspn(char const* s, char const* reject)
{
    for (auto* p = s;;) {
        char c = *p++;
        auto* rp = reject;
        char rc;
        do {
            if ((rc = *rp++) == c)
                return p - 1 - s;
        } while (rc);
    }
}

/**
 * @param str 
 * @return size_t 
 */
size_t strlen(char const* str)
{
    size_t len = 0;
    while (*(str++))
        ++len;
    return len;
}

/**
 * @param str 
 * @param maxlen 
 * @return size_t 
 */
size_t strnlen(char const* str, size_t maxlen)
{
    size_t len = 0;
    for (; len < maxlen && *str; str++)
        len++;
    return len;
}

/**
 * @param str 
 * @return char* 
 */
char* strdup(char const* str)
{
    size_t len = strlen(str);
    char* new_str = (char*)malloc(len + 1);
    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

/**
 * @param str 
 * @param maxlen 
 * @return char* 
 */
char* strndup(char const* str, size_t maxlen)
{
    size_t len = strnlen(str, maxlen);
    char* new_str = (char*)malloc(len + 1);
    memcpy(new_str, str, len);
    new_str[len] = 0;
    return new_str;
}

/**
 * @param s1 
 * @param s2 
 * @return int 
 */
int strcmp(char const* s1, char const* s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return 0;
    return *(unsigned char const*)s1 - *(unsigned char const*)--s2;
}

/**
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int strncmp(char const* s1, char const* s2, size_t n)
{
    if (!n)
        return 0;
    do {
        if (*s1 != *s2++)
            return *(unsigned char const*)s1 - *(unsigned char const*)--s2;
        if (*s1++ == 0)
            break;
    } while (--n);
    return 0;
}

/**
 * @param v1 
 * @param v2 
 * @param n 
 * @return int 
 */
int memcmp(void const* v1, void const* v2, size_t n)
{
    auto* s1 = (uint8_t const*)v1;
    auto* s2 = (uint8_t const*)v2;
    while (n-- > 0) {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}

/**
 * @param b1 
 * @param b2 
 * @param len 
 * @return int 
 */
int timingsafe_memcmp(void const* b1, void const* b2, size_t len)
{
    return Mods::timing_safe_compare(b1, b2, len) ? 1 : 0;
}

/**
 * @param dest_ptr 
 * @param src_ptr 
 * @param n 
 * @return void* 
 */
void* memcpy(void* dest_ptr, void const* src_ptr, size_t n)
{
    void* original_dest = dest_ptr;
    asm volatile(
        "rep movsb"
        : "+D"(dest_ptr), "+S"(src_ptr), "+c"(n)::"memory");
    return original_dest;
}

/**
 * @param dest_ptr 
 * @param c 
 * @param n 
 * @return void* 
 */
void* memset(void* dest_ptr, int c, size_t n)
{
    size_t dest = (size_t)dest_ptr;

    if (!(dest & 0x3) && n >= 12) {
        size_t size_ts = n / sizeof(size_t);
        size_t expanded_c = explode_byte((u8)c);
#if ARCH(I386)
        asm volatile(
            "rep stosl\n"
            : "=D"(dest)
            : "D"(dest), "c"(size_ts), "a"(expanded_c)
            : "memory");
#else
        asm volatile(
            "rep stosq\n"
            : "=D"(dest)
            : "D"(dest), "c"(size_ts), "a"(expanded_c)
            : "memory");
#endif
        n -= size_ts * sizeof(size_t);
        if (n == 0)
            return dest_ptr;
    }
    asm volatile(
        "rep stosb\n"
        : "=D"(dest), "=c"(n)
        : "0"(dest), "1"(n), "a"(c)
        : "memory");
    return dest_ptr;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return void* 
 */
void* memmove(void* dest, void const* src, size_t n)
{
    if (((FlatPtr)dest - (FlatPtr)src) >= n)
        return memcpy(dest, src, n);

    u8* pd = (u8*)dest;
    u8 const* ps = (u8 const*)src;
    for (pd += n, ps += n; n--;)
        *--pd = *--ps;
    return dest;
}

/**
 * @param haystack 
 * @param haystack_length 
 * @param needle 
 * @param needle_length 
 * @return void const* 
 */
void const* memmem(void const* haystack, size_t haystack_length, void const* needle, size_t needle_length)
{
    return Mods::memmem(haystack, haystack_length, needle, needle_length);
}

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcpy(char* dest, char const* src)
{
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return original_dest;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return char* 
 */
char* strncpy(char* dest, char const* src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i)
        dest[i] = src[i];
    for (; i < n; ++i)
        dest[i] = '\0';
    return dest;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return size_t 
 */
size_t strlcpy(char* dest, char const* src, size_t n)
{
    size_t i;

    for (i = 0; i + 1 < n && src[i] != '\0'; ++i)
        dest[i] = src[i];
    if (n)
        dest[i] = '\0';
    for (; src[i] != '\0'; ++i)
        ; // Determine the length of src, don't copy.
    return i;
}

/**
 * @param str 
 * @param c 
 * @return char* 
 */
char* strchr(char const* str, int c)
{
    char ch = c;
    for (;; ++str) {
        if (*str == ch)
            return const_cast<char*>(str);
        if (!*str)
            return nullptr;
    }
}

/**
 * @param str 
 * @param c 
 * @return char* 
 */
char* index(char const* str, int c)
{
    return strchr(str, c);
}

/**
 * @param str 
 * @param c 
 * @return char* 
 */
char* strchrnul(char const* str, int c)
{
    char ch = c;
    for (;; ++str) {
        if (*str == ch || !*str)
            return const_cast<char*>(str);
    }
}

/**
 * @param ptr 
 * @param c 
 * @param size 
 * @return void* 
 */
void* memchr(void const* ptr, int c, size_t size)
{
    char ch = c;
    auto* cptr = (char const*)ptr;
    for (size_t i = 0; i < size; ++i) {
        if (cptr[i] == ch)
            return const_cast<char*>(cptr + i);
    }
    return nullptr;
}

/**
 * @param str 
 * @param ch 
 * @return char* 
 */
char* strrchr(char const* str, int ch)
{
    char* last = nullptr;
    char c;
    for (; (c = *str); ++str) {
        if (c == ch)
            last = const_cast<char*>(str);
    }
    return last;
}

/**
 * @param str 
 * @param ch 
 * @return char* 
 */
char* rindex(char const* str, int ch)
{
    return strrchr(str, ch);
}

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcat(char* dest, char const* src)
{
    size_t dest_length = strlen(dest);
    size_t i;
    for (i = 0; src[i] != '\0'; i++)
        dest[dest_length + i] = src[i];
    dest[dest_length + i] = '\0';
    return dest;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return char* 
 */
char* strncat(char* dest, char const* src, size_t n)
{
    size_t dest_length = strlen(dest);
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[dest_length + i] = src[i];
    dest[dest_length + i] = '\0';
    return dest;
}

char const* const sys_errlist[] = {
#define __ENUMERATE_ERRNO_CODE(c, s) s,
    ENUMERATE_ERRNO_CODES(__ENUMERATE_ERRNO_CODE)
#undef __ENUMERATE_ERRNO_CODE
};
static_assert(array_size(sys_errlist) == (EMAXERRNO + 1));

int sys_nerr = EMAXERRNO;

/**
 * @param errnum 
 * @param buf 
 * @param buflen 
 * @return int 
 */
int strerror_r(int errnum, char* buf, size_t buflen)
{
    auto saved_errno = errno;
    if (errnum < 0 || errnum >= EMAXERRNO) {
        auto rc = strlcpy(buf, "unknown error", buflen);
        if (rc >= buflen)
            dbgln("strerror_r(): Invalid error number '{}' specified and the output buffer is too small.", errnum);
        errno = saved_errno;
        return EINVAL;
    }
    auto text = strerror(errnum);
    auto rc = strlcpy(buf, text, buflen);
    if (rc >= buflen) {
        errno = saved_errno;
        return ERANGE;
    }
    errno = saved_errno;
    return 0;
}

/**
 * @param errnum 
 * @return char* 
 */
char* strerror(int errnum)
{
    if (errnum < 0 || errnum >= EMAXERRNO) {
        return const_cast<char*>("Unknown error");
    }
    return const_cast<char*>(sys_errlist[errnum]);
}

/**
 * @param signum 
 * @return char* 
 */
char* strsignal(int signum)
{
    if (signum >= NSIG) {
        dbgln("strsignal() missing string for signum={}", signum);
        return const_cast<char*>("Unknown signal");
    }
    return const_cast<char*>(sys_siglist[signum]);
}

/**
 * @param haystack 
 * @param needle 
 * @return char* 
 */
char* strstr(char const* haystack, char const* needle)
{
    char nch;
    char hch;

    if ((nch = *needle++) != 0) {
        size_t len = strlen(needle);
        do {
            do {
                if ((hch = *haystack++) == 0)
                    return nullptr;
            } while (hch != nch);
        } while (strncmp(haystack, needle, len) != 0);
        --haystack;
    }
    return const_cast<char*>(haystack);
}

/**
 * @param s 
 * @param accept 
 * @return char* 
 */
char* strpbrk(char const* s, char const* accept)
{
    while (*s)
        if (strchr(accept, *s++))
            return const_cast<char*>(--s);
    return nullptr;
}

/**
 * @param str 
 * @param delim 
 * @param saved_str 
 * @return char* 
 */
char* strtok_r(char* str, char const* delim, char** saved_str)
{
    if (!str) {
        if (!saved_str || *saved_str == nullptr)
            return nullptr;
        str = *saved_str;
    }

    size_t token_start = 0;
    size_t token_end = 0;
    size_t str_len = strlen(str);
    size_t delim_len = strlen(delim);

    for (size_t i = 0; i < str_len; ++i) {
        bool is_proper_delim = false;

        for (size_t j = 0; j < delim_len; ++j) {
            if (str[i] == delim[j]) {
                if (token_end - token_start == 0) {
                    ++token_start;
                    break;
                }

                is_proper_delim = true;
            }
        }

        ++token_end;
        if (is_proper_delim && token_end > 0) {
            --token_end;
            break;
        }
    }

    if (str[token_start] == '\0') {
        *saved_str = nullptr;
        return nullptr;
    }

    if (token_end == 0) {
        *saved_str = nullptr;
        return &str[token_start];
    }

    if (str[token_end] == '\0')
        *saved_str = &str[token_end];
    else
        *saved_str = &str[token_end + 1];

    str[token_end] = '\0';
    return &str[token_start];
}

/**
 * @param str 
 * @param delim 
 * @return char* 
 */
char* strtok(char* str, char const* delim)
{
    static char* saved_str;
    return strtok_r(str, delim, &saved_str);
}

/**
 * @param s1 
 * @param s2 
 * @return int 
 */
int strcoll(char const* s1, char const* s2)
{
    return strcmp(s1, s2);
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return size_t 
 */
size_t strxfrm(char* dest, char const* src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i)
        dest[i] = src[i];
    for (; i < n; ++i)
        dest[i] = '\0';
    return i;
}

/**
 * @param str 
 * @param delim 
 * @return char* 
 */
char* strsep(char** str, char const* delim)
{
    if (*str == nullptr)
        return nullptr;
    auto* begin = *str;
    auto* end = begin + strcspn(begin, delim);
    if (*end) {
        *end = '\0';
        *str = ++end;
    } else {
        *str = nullptr;
    }
    return begin;
}

/**
 * @param ptr 
 * @param size 
 */
void explicit_bzero(void* ptr, size_t size)
{
    secure_zero(ptr, size);
}
}
