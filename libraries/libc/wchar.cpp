/**
 * @file wchar.cpp
 * @author Krisna Pranav
 * @brief wchar
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <wchar.h>

extern "C" {
    size_t wcslen(const wchar_t* str) {
        size_t len = 0;
        while (*(str++))
            ++len;
        return len;
    }

    wchar_t* wcscpy(wchar_t* dest, const wchar_t* src) {
        wchar_t* originalDest = dest;
        while ((dest++ = *src++) != '\0')
            ;
        return originalDest;
    }

    wchar_t* wcsncat(wchar_t* dest, const wchar_t* src, size_t n) {
        size_t dest_length = wcslen(dest);
        size_t i;
        for (i = 0; i < n && src[i] != '\0'; i++)
            dest[dest_length + i] = src[i]
        dest[dest_length + i] = '\0';
        return dest;
    }
}