/**
 * @file stdio.h
 * @author Krisna Pranav
 * @brief STDIO
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define _STDIO_H 

#include <kernel/api/posix/stdio.h>
#include <bits/FILE.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define FILENAME_MAX 1024
#define FOPEN_MAX 1024

__BEGIN_DECLS
#ifndef EOF
#    define EOF (-1)
#endif

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define L_tmpnam 256
#define P_tmpdir "/tmp"

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

typedef off_t fpos_t;

/**
 * @param offset 
 * @param whence 
 * @return int 
 */
int fseek(FILE*, long offset, int whence);

/**
 * @param offset 
 * @param whence 
 * @return int 
 */
int fseeko(FILE*, off_t offset, int whence);

/**
 * @return int 
 */
int fgetpos(FILE*, fpos_t*);

int fsetpos(FILE*, fpos_t const*);

long ftell(FILE*);

off_t ftello(FILE*);

/**
 * @param buffer 
 * @param size 
 * @return char* 
 */
char* fgets(char* buffer, int size, FILE*);

/**
 * @param ch 
 * @return int 
 */
int fputc(int ch, FILE*);

int fileno(FILE*);

int fgetc(FILE*);

int fgetc_unlocked(FILE*);

int getc(FILE*);

/**
 * @param stream 
 * @return int 
 */
int getc_unlocked(FILE* stream);

int getchar(void);

/**
 * @return ssize_t 
 */
ssize_t getdelim(char**, size_t*, int, FILE*);

/**
 * @return ssize_t 
 */
ssize_t getline(char**, size_t*, FILE*);

/**
 * @param c 
 * @return int 
 */
int ungetc(int c, FILE*);

/**
 * @param pathname 
 * @return int 
 */
int remove(char const* pathname);

/**
 * @param fd 
 * @param mode 
 * @return FILE* 
 */
FILE* fdopen(int fd, char const* mode);

/**
 * @param pathname 
 * @param mode 
 * @return FILE* 
 */
FILE* fopen(char const* pathname, char const* mode);

/**
 * @param pathname 
 * @param mode 
 * @return FILE* 
 */
FILE* freopen(char const* pathname, char const* mode, FILE*);

/**
 * @param buf 
 * @param size 
 * @param mode 
 * @return FILE* 
 */
FILE* fmemopen(void* buf, size_t size, char const* mode);

/**
 * @param filehandle 
 */
void flockfile(FILE* filehandle);

/**
 * @param filehandle 
 */
void funlockfile(FILE* filehandle);

/**
 * @return int 
 */
int fclose(FILE*);

void rewind(FILE*);

void clearerr(FILE*);

int ferror(FILE*);

int feof(FILE*);

int fflush(FILE*);

/**
 * @param ptr 
 * @param size 
 * @param nmemb 
 * @return size_t 
 */
size_t fread(void* ptr, size_t size, size_t nmemb, FILE*);

/**
 * @param ptr 
 * @param size 
 * @param nmemb 
 * @return size_t 
 */
size_t fread_unlocked(void* ptr, size_t size, size_t nmemb, FILE*);

/**
 * @param ptr 
 * @param size 
 * @param nmemb 
 * @return size_t 
 */
size_t fwrite(void const* ptr, size_t size, size_t nmemb, FILE*);

int vprintf(char const* fmt, va_list) __attribute__((format(printf, 1, 0)));
int vfprintf(FILE*, char const* fmt, va_list) __attribute__((format(printf, 2, 0)));
int vasprintf(char** strp, char const* fmt, va_list) __attribute__((format(printf, 2, 0)));
int vsprintf(char* buffer, char const* fmt, va_list) __attribute__((format(printf, 2, 0)));
int vsnprintf(char* buffer, size_t, char const* fmt, va_list) __attribute__((format(printf, 3, 0)));
int fprintf(FILE*, char const* fmt, ...) __attribute__((format(printf, 2, 3)));
int printf(char const* fmt, ...) __attribute__((format(printf, 1, 2)));
void dbgputstr(char const*, size_t);
int sprintf(char* buffer, char const* fmt, ...) __attribute__((format(printf, 2, 3)));
int asprintf(char** strp, char const* fmt, ...) __attribute__((format(printf, 2, 3)));
int snprintf(char* buffer, size_t, char const* fmt, ...) __attribute__((format(printf, 3, 4)));
int putchar(int ch);
int putc(int ch, FILE*);
int puts(char const*);
int fputs(char const*, FILE*);
void perror(char const*);
int scanf(char const* fmt, ...) __attribute__((format(scanf, 1, 2)));
int sscanf(char const* str, char const* fmt, ...) __attribute__((format(scanf, 2, 3)));
int fscanf(FILE*, char const* fmt, ...) __attribute__((format(scanf, 2, 3)));
int vscanf(char const*, va_list) __attribute__((format(scanf, 1, 0)));
int vfscanf(FILE*, char const*, va_list) __attribute__((format(scanf, 2, 0)));
int vsscanf(char const*, char const*, va_list) __attribute__((format(scanf, 2, 0)));
int setvbuf(FILE*, char* buf, int mode, size_t);
void setbuf(FILE*, char* buf);
void setlinebuf(FILE*);
int rename(char const* oldpath, char const* newpath);
FILE* tmpfile(void);
char* tmpnam(char*);
FILE* popen(char const* command, char const* type);
int pclose(FILE*);

__END_DECLS
