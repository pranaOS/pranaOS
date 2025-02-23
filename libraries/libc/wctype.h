/**
 * @file wctype.h
 * @author Krisna Pranav
 * @brief wctype
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <assert.h>
#include <ctype.h>
#include <wchar.h>

__BEGIN_DECLS

typedef long wctrans_t;

/**
 * @param wc 
 * @return int 
 */
int iswalnum(wint_t wc);
int iswalpha(wint_t wc);
int iswcntrl(wint_t wc);
int iswdigit(wint_t wc);
int iswxdigit(wint_t wc);
int iswspace(wint_t wc);
int iswpunct(wint_t wc);
int iswprint(wint_t wc);
int iswgraph(wint_t wc);
int iswlower(wint_t wc);
int iswupper(wint_t wc);
int iswblank(wint_t wc);

/**
 * @return int 
 */
int iswctype(wint_t, wctype_t);

/**
 * @return wctype_t 
 */
wctype_t wctype(char const*);

/**
 * @param wc 
 * @return wint_t 
 */
wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);

/**
 * @return wint_t 
 */
wint_t towctrans(wint_t, wctrans_t);

/**
 * @return wctrans_t 
 */
wctrans_t wctrans(char const*);

__END_DECLS
