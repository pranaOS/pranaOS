//
// Created by KrisnaPranav on 02/02/22.
//

#pragma once

#include <sys/types.h>

/**
 * @brief locale_t
 *
 */
#ifndef LOCALE_T
#define LOCALE_T
typedef __locale_t locale_t;
#endif

/**
 * @brief alpha, alnum, ascii, blank, cntrl, digit, graph, lower, print, space, punct, upper, xdigit, ascii, lower
 */

int isalnum(int);
int isalpha(int);
int isascii(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int toascii(int);
int tolower(int);
int toupper(int);