/**
 * @file stdio.h
 * @author Krisna Pranav
 * @brief STDIO
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <cstdio>
#define _STDIO_H

#include <limits.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define FILENAME_MAX 1024

__BEGIN_DECLS

#ifndef EOF
#   define EOF(-1)
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

typedef long fpos_t;

/**
 * @param offset 
 * @param whence 
 * @return int 
 */
int fseek(FILE*, long offset, int whence);

/**
 * @return int 
 */
int fgetpos(FILE*, fpos_t);

/**
 * @return int 
 */
int fsetpos(FILE*, const fpos_t);

/**
 * @return long 
 */
long ftell(FILE *);

/**
 * @param buffer 
 * @param size 
 * @return char* 
 */
char* fgets(char* buffer, int size, FILE*);

int fputc(int ch, FILE*);
int fileno(FILE*);
int fgetc(FILE*);
int getc(FILE*);

__END_DECLS