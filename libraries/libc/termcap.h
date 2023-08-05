/**
 * @file termcap.h
 * @author Krisna Pranav
 * @brief termcap
 * @version 1.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

extern char* PC;
extern char* UP;
extern char* BC;

/**
 * @param bp 
 * @param name 
 * @return int 
 */
int tgetent(char* bp, const char* name);

/**
 * @param id 
 * @return int 
 */
int tgetflag(const char* id);

/**
 * @param id 
 * @return int 
 */
int tgetnum(const char* id);

/**
 * @param id 
 * @param area 
 * @return char* 
 */
char* tgetstr(const char* id, char** area);

/**
 * @param cap 
 * @param col 
 * @param row 
 * @return char* 
 */
char* tgoto(const char* cap, int col, int row);