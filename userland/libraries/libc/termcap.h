/**
 * @file termcap.h
 * @author Krisna Pranav
 * @brief termcap
 * @version 6.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

extern char PC;
extern char* UP;
extern char* BC;

/**
 * @param bp 
 * @param name 
 * @return int 
 */
int tgetent(char* bp, char const* name);

/**
 * @param id 
 * @return int 
 */
int tgetflag(char const* id);

/**
 * @param id 
 * @return int 
 */
int tgetnum(char const* id);

/**
 * @param id 
 * @param area 
 * @return char* 
 */
char* tgetstr(char const* id, char** area);

/**
 * @param cap 
 * @param col 
 * @param row 
 * @return char* 
 */
char* tgoto(char const* cap, int col, int row);

/**
 * @param str 
 * @param affcnt 
 * @param putc 
 * @return int 
 */
int tputs(char const* str, int affcnt, int (*putc)(int));

__END_DECLS
