/**
 * @file wctype.cpp
 * @author Krisna Pranav
 * @brief wctype
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <string.h>
#include <wctype.h>

enum {
    WCTYPE_INVALID = 0,
    WCTYPE_ALNUM,
    WCTYPE_ALPHA,
    WCTYPE_BLANK,
    WCTYPE_CNTRL,
    WCTYPE_DIGIT,
    WCTYPE_GRAPH,
    WCTYPE_LOWER,
    WCTYPE_PRINT,
    WCTYPE_PUNCT,
    WCTYPE_SPACE,
    WCTYPE_UPPER,
    WCTYPE_XDIGIT,
}; // enum

enum {
    WCTRANS_INVALID = 0,
    WCTRANS_TOLOWER,
    WCTRANS_TOUPPER,
}; // enum

extern "C" {

/**
 * @param wc 
 * @return int 
 */
int iswalnum(wint_t wc)
{
    return __inline_isalnum(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswalpha(wint_t wc)
{
    return __inline_isalpha(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswcntrl(wint_t wc)
{
    return __inline_iscntrl(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswdigit(wint_t wc)
{
    return __inline_isdigit(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswxdigit(wint_t wc)
{
    return __inline_isxdigit(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswspace(wint_t wc)
{
    return __inline_isspace(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswpunct(wint_t wc)
{
    return __inline_ispunct(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswprint(wint_t wc)
{
    return __inline_isprint(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswgraph(wint_t wc)
{
    return __inline_isgraph(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswlower(wint_t wc)
{
    return __inline_islower(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswupper(wint_t wc)
{
    return __inline_isupper(wc);
}

/**
 * @param wc 
 * @return int 
 */
int iswblank(wint_t wc)
{
    return __inline_isblank(wc);
}

/**
 * @param wc 
 * @param charclass 
 * @return int 
 */
int iswctype(wint_t wc, wctype_t charclass)
{
    switch (charclass) {
    case WCTYPE_ALNUM:
        return iswalnum(wc);

    case WCTYPE_ALPHA:
        return iswalpha(wc);

    case WCTYPE_BLANK:
        return iswblank(wc);

    case WCTYPE_CNTRL:
        return iswcntrl(wc);

    case WCTYPE_DIGIT:
        return iswdigit(wc);

    case WCTYPE_GRAPH:
        return iswgraph(wc);

    case WCTYPE_LOWER:
        return iswlower(wc);

    case WCTYPE_PRINT:
        return iswprint(wc);

    case WCTYPE_PUNCT:
        return iswpunct(wc);

    case WCTYPE_SPACE:
        return iswspace(wc);

    case WCTYPE_UPPER:
        return iswupper(wc);

    case WCTYPE_XDIGIT:
        return iswxdigit(wc);

    default:
        return 0;
    }
}

/**
 * @param property 
 * @return wctype_t 
 */
wctype_t wctype(char const* property)
{
    if (strcmp(property, "alnum") == 0)
        return WCTYPE_ALNUM;

    if (strcmp(property, "alpha") == 0)
        return WCTYPE_ALPHA;

    if (strcmp(property, "blank") == 0)
        return WCTYPE_BLANK;

    if (strcmp(property, "cntrl") == 0)
        return WCTYPE_CNTRL;

    if (strcmp(property, "digit") == 0)
        return WCTYPE_DIGIT;

    if (strcmp(property, "graph") == 0)
        return WCTYPE_GRAPH;

    if (strcmp(property, "lower") == 0)
        return WCTYPE_LOWER;

    if (strcmp(property, "print") == 0)
        return WCTYPE_PRINT;

    if (strcmp(property, "punct") == 0)
        return WCTYPE_PUNCT;

    if (strcmp(property, "space") == 0)
        return WCTYPE_SPACE;

    if (strcmp(property, "upper") == 0)
        return WCTYPE_UPPER;

    if (strcmp(property, "xdigit") == 0)
        return WCTYPE_XDIGIT;

    return WCTYPE_INVALID;
}

/**
 * @param wc 
 * @return wint_t 
 */
wint_t towlower(wint_t wc)
{
    return __inline_tolower(wc);
}

/**
 * @param wc 
 * @return wint_t 
 */
wint_t towupper(wint_t wc)
{
    return __inline_toupper(wc);
}

/**
 * @param wc 
 * @param desc 
 * @return wint_t 
 */
wint_t towctrans(wint_t wc, wctrans_t desc)
{
    switch (desc) {
    case WCTRANS_TOLOWER:
        return towlower(wc);

    case WCTRANS_TOUPPER:
        return towupper(wc);

    default:
        return wc;
    }
}

/**
 * @param charclass 
 * @return wctrans_t 
 */
wctrans_t wctrans(char const* charclass)
{
    if (strcmp(charclass, "tolower") == 0)
        return WCTRANS_TOLOWER;

    if (strcmp(charclass, "toupper") == 0)
        return WCTRANS_TOUPPER;

    return WCTRANS_INVALID;
}
}
