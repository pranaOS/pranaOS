/**
 * @file wchar.cpp
 * @author Krisna Pranav
 * @brief wchar
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/assertions.h>
#include <mods/format.h>
#include <mods/unicodeutils.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

/**
 * @param state 
 * @return unsigned int 
 */
static unsigned int mbstate_expected_bytes(mbstate_t* state)
{
    if (state->stored_bytes == 0) {
        return 0;
    }

    unsigned char first = state->bytes[0];

    if ((first & 0b10000000) == 0) {
        return 1;
    }

    if ((first & 0b11100000) == 0b11000000) {
        return 2;
    }

    if ((first & 0b11110000) == 0b11100000) {
        return 3;
    }

    if ((first & 0b11111000) == 0b11110000) {
        return 4;
    }

    return 0;
}

extern "C" {

/**
 * @param str 
 * @return size_t 
 */
size_t wcslen(wchar_t const* str)
{
    size_t len = 0;
    while (*(str++))
        ++len;
    return len;
}

/**
 * @param dest 
 * @param src 
 * @return wchar_t* 
 */
wchar_t* wcscpy(wchar_t* dest, wchar_t const* src)
{
    wchar_t* original_dest = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return original_dest;
}

/**
 * @param str 
 * @return wchar_t* 
 */
wchar_t* wcsdup(wchar_t const* str)
{
    size_t length = wcslen(str);
    wchar_t* new_str = (wchar_t*)malloc(sizeof(wchar_t) * (length + 1));

    if (!new_str) {
        errno = ENOMEM;
        return nullptr;
    }

    return wcscpy(new_str, str);
}

/**
 * @param dest 
 * @param src 
 * @param num 
 * @return wchar_t* 
 */
wchar_t* wcsncpy(wchar_t* dest, wchar_t const* src, size_t num)
{
    wchar_t* original_dest = dest;
    while (((*dest++ = *src++) != '\0') && ((size_t)(dest - original_dest) < num))
        ;
    return original_dest;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return size_t 
 */
size_t wcslcpy(wchar_t* dest, wchar_t const* src, size_t n)
{
    size_t i;
    for (i = 0; i + 1 < n && src[i] != L'\0'; ++i)
        dest[i] = src[i];
    if (n)
        dest[i] = L'\0';
    for (; src[i] != L'\0'; ++i)
        ; 
    return i;
}

/**
 * @param s1 
 * @param s2 
 * @return int 
 */
int wcscmp(wchar_t const* s1, wchar_t const* s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return 0;
    return *(wchar_t const*)s1 - *(wchar_t const*)--s2;
}

/**
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int wcsncmp(wchar_t const* s1, wchar_t const* s2, size_t n)
{
    if (!n)
        return 0;
    do {
        if (*s1 != *s2++)
            return *(wchar_t const*)s1 - *(wchar_t const*)--s2;
        if (*s1++ == 0)
            break;
    } while (--n);
    return 0;
}

/**
 * @param str 
 * @param c 
 * @return wchar_t* 
 */
wchar_t* wcschr(wchar_t const* str, int c)
{
    wchar_t ch = c;
    for (;; ++str) {
        if (*str == ch)
            return const_cast<wchar_t*>(str);
        if (!*str)
            return nullptr;
    }
}

/**
 * @param str 
 * @param wc 
 * @return wchar_t* 
 */
wchar_t* wcsrchr(wchar_t const* str, wchar_t wc)
{
    wchar_t* last = nullptr;
    wchar_t c;
    for (; (c = *str); ++str) {
        if (c == wc)
            last = const_cast<wchar_t*>(str);
    }
    return last;
}

/**
 * @param dest 
 * @param src 
 * @return wchar_t* 
 */
wchar_t* wcscat(wchar_t* dest, wchar_t const* src)
{
    size_t dest_length = wcslen(dest);
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
 * @return wchar_t* 
 */
wchar_t* wcsncat(wchar_t* dest, wchar_t const* src, size_t n)
{
    size_t dest_length = wcslen(dest);
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[dest_length + i] = src[i];
    dest[dest_length + i] = '\0';
    return dest;
}

/**
 * @param str 
 * @param delim 
 * @param ptr 
 * @return wchar_t* 
 */
wchar_t* wcstok(wchar_t* str, wchar_t const* delim, wchar_t** ptr)
{
    wchar_t* used_str = str;
    if (!used_str) {
        used_str = *ptr;
    }

    size_t token_start = 0;
    size_t token_end = 0;
    size_t str_len = wcslen(used_str);
    size_t delim_len = wcslen(delim);

    for (size_t i = 0; i < str_len; ++i) {
        bool is_proper_delim = false;

        for (size_t j = 0; j < delim_len; ++j) {
            if (used_str[i] == delim[j]) {
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

    if (used_str[token_start] == '\0')
        return nullptr;

    if (token_end == 0) {
        return &used_str[token_start];
    }

    used_str[token_end] = '\0';
    return &used_str[token_start];
}

/**
 * @return long 
 */
long wcstol(wchar_t const*, wchar_t**, int)
{
    dbgln("FIXME: Implement wcstol()");
    TODO();
}

/**
 * @return long long 
 */
long long wcstoll(wchar_t const*, wchar_t**, int)
{
    dbgln("FIXME: Implement wcstoll()");
    TODO();
}

/**
 * @param c 
 * @return wint_t 
 */
wint_t btowc(int c)
{
    if (c == EOF) {
        return WEOF;
    }

    if (c & (1 << 7)) {
        return WEOF;
    }

    return c;
}

/**
 * @param pwc 
 * @param s 
 * @param n 
 * @param state 
 * @return size_t 
 */
size_t mbrtowc(wchar_t* pwc, char const* s, size_t n, mbstate_t* state)
{
    static mbstate_t _anonymous_state = {};

    if (state == nullptr) {
        state = &_anonymous_state;
    }

    if (s == nullptr) {
        pwc = nullptr;
        s = "";
        n = 1;
    }

    if (n == 0) {
        return 0;
    }

    size_t consumed_bytes = 0;

    if (state->stored_bytes == 0) {
        state->bytes[state->stored_bytes++] = s[0];
        consumed_bytes++;
    }

    size_t expected_bytes = mbstate_expected_bytes(state);

    if (expected_bytes == 0) {
        *state = {};
        errno = EILSEQ;
        return -1;
    }

    while (state->stored_bytes < expected_bytes) {
        if (consumed_bytes == n) {
            return -2;
        }

        unsigned char c = s[consumed_bytes];

        if ((c & 0b11000000) != 0b10000000) {
            *state = {};
            errno = EILSEQ;
            return -1;
        }

        state->bytes[state->stored_bytes++] = c;
        consumed_bytes++;
    }

    wchar_t codepoint = state->bytes[0];

    if (expected_bytes > 1) {
        codepoint &= (1 << (7 - expected_bytes)) - 1;
    }

    for (unsigned int i = 1; i < expected_bytes; i++) {
        codepoint = codepoint << 6;
        codepoint |= state->bytes[i] & 0b111111;
    }

    if (pwc) {
        *pwc = codepoint;
    }

    state->stored_bytes = 0;

    if (codepoint == 0) {
        *state = {};
        return 0;
    }

    return consumed_bytes;
}

/**
 * @param s 
 * @param n 
 * @param ps 
 * @return size_t 
 */
size_t mbrlen(char const* s, size_t n, mbstate_t* ps)
{
    static mbstate_t anonymous_state = {};

    if (ps == nullptr)
        ps = &anonymous_state;

    return mbrtowc(nullptr, s, n, ps);
}

/**
 * @param s 
 * @param wc 
 * @return size_t 
 */
size_t wcrtomb(char* s, wchar_t wc, mbstate_t*)
{
    if (s == nullptr)
        wc = L'\0';

    auto nwritten = Mods::UnicodeUtils::code_point_to_utf8(wc, [&s](char byte) {
        if (s != nullptr)
            *s++ = byte;
    });

    if (nwritten < 0) {
        errno = EILSEQ;
        return (size_t)-1;
    } else {
        return nwritten;
    }
}

/**
 * @param ws1 
 * @param ws2 
 * @return int 
 */
int wcscoll(wchar_t const* ws1, wchar_t const* ws2)
{
    return wcscmp(ws1, ws2);
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return size_t 
 */
size_t wcsxfrm(wchar_t* dest, wchar_t const* src, size_t n)
{
    return wcslcpy(dest, src, n);
}

/**
 * @param c 
 * @return int 
 */
int wctob(wint_t c)
{
    if (c > 0x7f)
        return EOF;

    return static_cast<unsigned char>(c);
}

/**
 * @param state 
 * @return int 
 */
int mbsinit(mbstate_t const* state)
{
    if (!state) {
        return 1;
    }

    if (state->stored_bytes != 0) {
        return 0;
    }

    return 1;
}

/**
 * @param wcs 
 * @param accept 
 * @return wchar_t* 
 */
wchar_t* wcspbrk(wchar_t const* wcs, wchar_t const* accept)
{
    for (wchar_t const* cur = accept; *cur; cur++) {
        wchar_t* res = wcschr(wcs, *cur);
        if (res)
            return res;
    }

    return nullptr;
}

/**
 * @param haystack 
 * @param needle 
 * @return wchar_t* 
 */
wchar_t* wcsstr(wchar_t const* haystack, wchar_t const* needle)
{
    size_t nlen = wcslen(needle);

    if (nlen == 0)
        return const_cast<wchar_t*>(haystack);

    size_t hlen = wcslen(haystack);

    while (hlen >= nlen) {
        if (wcsncmp(haystack, needle, nlen) == 0)
            return const_cast<wchar_t*>(haystack);

        haystack++;
        hlen--;
    }

    return nullptr;
}

/**
 * @param s 
 * @param c 
 * @param n 
 * @return wchar_t* 
 */
wchar_t* wmemchr(wchar_t const* s, wchar_t c, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (s[i] == c)
            return const_cast<wchar_t*>(&s[i]);
    }

    return nullptr;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return wchar_t* 
 */
wchar_t* wmemcpy(wchar_t* dest, wchar_t const* src, size_t n)
{
    for (size_t i = 0; i < n; i++)
        dest[i] = src[i];

    return dest;
}

/**
 * @param wcs 
 * @param wc 
 * @param n 
 * @return wchar_t* 
 */
wchar_t* wmemset(wchar_t* wcs, wchar_t wc, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        wcs[i] = wc;
    }

    return wcs;
}

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return wchar_t* 
 */
wchar_t* wmemmove(wchar_t* dest, wchar_t const* src, size_t n)
{
    if (dest > src) {
        for (size_t i = 1; i <= n; i++) {
            dest[n - i] = src[n - i];
        }
    } else if (dest < src) {
        for (size_t i = 0; i < n; i++) {
            dest[i] = src[i];
        }
    }

    return dest;
}

/**
 * @return unsigned long 
 */
unsigned long wcstoul(wchar_t const*, wchar_t**, int)
{
    dbgln("TODO: Implement wcstoul()");
    TODO();
}

/**
 * @return unsigned long long 
 */
unsigned long long wcstoull(wchar_t const*, wchar_t**, int)
{
    dbgln("TODO: Implement wcstoull()");
    TODO();
}

/**
 * @return float 
 */
float wcstof(wchar_t const*, wchar_t**)
{
    dbgln("TODO: Implement wcstof()");
    TODO();
}

/**
 * @return double 
 */
double wcstod(wchar_t const*, wchar_t**)
{
    dbgln("TODO: Implement wcstod()");
    TODO();
}

/**
 * @return long double 
 */
long double wcstold(wchar_t const*, wchar_t**)
{
    dbgln("TODO: Implement wcstold()");
    TODO();
}

/**
 * @param wc 
 * @return int 
 */
int wcwidth(wchar_t wc)
{
    if (wc == L'\0')
        return 0;

    if (wc >= 0x20 && wc <= 0x7e)
        return 1;

    if (wc <= 0x7f)
        return -1;

    return 1;
}

/**
 * @param dest 
 * @param src 
 * @param nwc 
 * @param len 
 * @param ps 
 * @return size_t 
 */
size_t wcsnrtombs(char* dest, wchar_t const** src, size_t nwc, size_t len, mbstate_t* ps)
{
    static mbstate_t _anonymous_state = {};

    if (ps == nullptr)
        ps = &_anonymous_state;

    size_t written = 0;
    size_t read = 0;
    while (read < nwc) {
        size_t ret = 0;
        char buf[MB_LEN_MAX];

        ret = wcrtomb(buf, **src, ps);

        if (ret == (size_t)-1) {
            errno = EILSEQ;
            return (size_t)-1;
        }

        if (dest && len < written + ret) {
            return written;
        }

        if (dest) {
            memcpy(dest, buf, ret);
            dest += ret;
        }

        if (**src == L'\0') {
            *src = nullptr;
            return written;
        }

        *src += 1;
        read += 1;
        written += ret;
    }

    return written;
}

/**
 * @param dst 
 * @param src 
 * @param nms 
 * @param len 
 * @param ps 
 * @return size_t 
 */
size_t mbsnrtowcs(wchar_t* dst, char const** src, size_t nms, size_t len, mbstate_t* ps)
{
    static mbstate_t _anonymous_state = {};

    if (ps == nullptr)
        ps = &_anonymous_state;

    size_t written = 0;
    while (written < len || !dst) {
        if (nms == 0) {
            return written;
        }

        size_t ret = mbrtowc(dst, *src, nms, ps);

        if (ret == -2ul) {
            *src += nms;
            return written;
        }

        if (ret == -1ul) {
            errno = EILSEQ;
            return (size_t)-1;
        }

        if (**src == '\0') {
            *src = nullptr;
            return written;
        }

        *src += ret;
        nms -= ret;
        written += 1;
        if (dst)
            dst += 1;
    }

    return written;
}

/**
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int wmemcmp(wchar_t const* s1, wchar_t const* s2, size_t n)
{
    while (n-- > 0) {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}

/**
 * @param dest 
 * @param src 
 * @param len 
 * @param ps 
 * @return size_t 
 */
size_t wcsrtombs(char* dest, wchar_t const** src, size_t len, mbstate_t* ps)
{
    static mbstate_t anonymous_state = {};

    if (ps == nullptr)
        ps = &anonymous_state;

    return wcsnrtombs(dest, src, SIZE_MAX, len, ps);
}

/**
 * @param dst 
 * @param src 
 * @param len 
 * @param ps 
 * @return size_t 
 */
size_t mbsrtowcs(wchar_t* dst, char const** src, size_t len, mbstate_t* ps)
{
    static mbstate_t anonymous_state = {};

    if (ps == nullptr)
        ps = &anonymous_state;

    return mbsnrtowcs(dst, src, SIZE_MAX, len, ps);
}

/**
 * @param wcs 
 * @param reject 
 * @return size_t 
 */
size_t wcscspn(wchar_t const* wcs, wchar_t const* reject)
{
    for (auto const* wc_pointer = wcs;;) {
        auto c = *wc_pointer++;
        wchar_t rc;
        auto const* reject_copy = reject;
        do {
            if ((rc = *reject_copy++) == c)
                return wc_pointer - 1 - wcs;
        } while (rc != 0);
    }
}

/**
 * @param wcs 
 * @param accept 
 * @return size_t 
 */
size_t wcsspn(wchar_t const* wcs, wchar_t const* accept)
{
    for (auto const* wc_pointer = wcs;;) {
        auto c = *wc_pointer++;
        wchar_t rc;
        auto const* accept_copy = accept;
        do {
            if ((rc = *accept_copy++) != c)
                return wc_pointer - 1 - wcs;
        } while (rc != 0);
    }
}

/**
 * @param wcs 
 * @param maxsize 
 * @param format 
 * @param timeptr 
 * @return size_t 
 */
size_t wcsftime(wchar_t* __restrict wcs, size_t maxsize, wchar_t const* __restrict format, const struct tm* __restrict timeptr)
{
    (void)wcs;
    (void)maxsize;
    (void)format;
    (void)timeptr;
    dbgln("FIXME: Implement wcsftime()");
    TODO();
}
}
