/**
 * @file wchar.h
 * @author Krisna Pranav
 * @brief wchar 
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

// WEOF
#ifndef WEOF
#    define WEOF (0xffffffffu)
#endif

/**
 * @return size_t 
 */
size_t wcslen(const wchar_t*);

/**
 * @return wchar_t* 
 */
wchar_t* wcscpy(wchar_t*, const wchar_t*);

/**
 * @return wchar_t* 
 */
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);

/**
 * @return int 
 */
int wcscmp(const wchar_t*, const wchar_t*);

/**
 * @return wchar_t* 
 */
wchar_t* wcschr(const wchar_t*, int);

/**
 * @return const wchar_t* 
 */
const wchar_t* wcsrchr(const wchar_t*, wchar_t);

/**
 * @return wchar_t* 
 */
wchar_t* wcscat(wchar_t*, const wchar_t*);

/**
 * @return wchar_t* 
 */
wchar_t* wcstok(wchar_t*, const wchar_t*, wchar_t**);

/**
 * @return wchar_t* 
 */
wchar_t* wcsncat(wchar_t*, const wchar_t*, size_t);

__END_DECLS