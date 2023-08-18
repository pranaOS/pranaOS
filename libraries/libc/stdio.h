/**
 * @file stdio.h
 * @author Krisna Pranav
 * @brief STDIO
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#define _STDIO_H 

#include <limits.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/file.h>

#define FILENAME_MAX 1024

__BEGIN_DECLS
#ifndef EOF
#    define EOF (-1)
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define L_tmpnam 256

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
int fgetpos(FILE*, fpos_t*);

/**
 * @return int 
 */
int fsetpos(FILE*, const fpos_t*);

/**
 * @return long 
 */
long ftell(FILE*);

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

/**
 * @return int 
 */
int fileno(FILE*);

/**
 * @return int 
 */
int fgetc(FILE*);

/**
 * @return int 
 */
int getc(FILE*);

/**
 * @param stream 
 * @return int 
 */
int getc_unlocked(FILE* stream);

/**
 * @return int 
 */
int getchar();

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
int remove(const char* pathname);

/**
 * @param fd 
 * @param mode 
 * @return FILE* 
 */
FILE* fdopen(int fd, const char* mode);

/**
 * @param pathname 
 * @param mode 
 * @return FILE* 
 */
FILE* fopen(const char* pathname, const char* mode);

/**

 * @param pathname 
 * @param mode 
 * @return FILE* 
 */
FILE* freopen(const char* pathname, const char* mode, FILE*);

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

/// @brief: rewind(FILE)
void rewind(FILE*);

/// @brief: clearerr
void clearerr(FILE*);

/**
 * @return int 
 */
int ferror(FILE*);

/**
 * @return int 
 */
int feof(FILE*);

/**
 * @return int 
 */
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
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE*);

/**
 * @param fmt 
 * @return int 
 */
int vprintf(const char* fmt, va_list);

/**
 * @param fmt 
 * @return int 
 */
int vfprintf(FILE*, const char* fmt, va_list);

/**
 * @param buffer 
 * @param fmt 
 * @return int 
 */
int vsprintf(char* buffer, const char* fmt, va_list);

/**
 * @param buffer 
 * @param fmt 
 * @return int 
 */
int vsnprintf(char* buffer, size_t, const char* fmt, va_list);

/**
 * @param fmt 
 * @param ... 
 * @return int 
 */
int fprintf(FILE*, const char* fmt, ...);

/**
 * @param fmt 
 * @param ... 
 * @return int 
 */
int printf(const char* fmt, ...);

/**
 * @param fmt 
 * @param ... 
 * @return int 
 */
int dbgprintf(const char* fmt, ...);

/// @brief: dbgputch
void dbgputch(char);

/**
 * @return int 
 */
int dbgputstr(const char*, ssize_t);

/**
 * @param buffer 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sprintf(char* buffer, const char* fmt, ...);

/**
 * @param buffer 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int snprintf(char* buffer, size_t, const char* fmt, ...);

/**
 * @param ch 
 * @return int 
 */
int putchar(int ch);

/**
 * @param ch 
 * @return int 
 */
int putc(int ch, FILE*);

/**
 * @return int 
 */
int puts(const char*);

/**
 * @return int 
 */
int fputs(const char*, FILE*);

/// @brief: perror
void perror(const char*);

/**
 * @param fmt 
 * @param ... 
 * @return int 
 */
int scanf(const char* fmt, ...);

/**
 * @param str 
 * @param fmt 
 * @param ... 
 * @return int 
 */
int sscanf(const char* str, const char* fmt, ...);

/**
 * @param fmt 
 * @param ... 
 * @return int 
 */
int fscanf(FILE*, const char* fmt, ...);

/**
 * @return int 
 */
int vfscanf(FILE*, const char*, va_list);

/**
 * @return int 
 */
int vsscanf(const char*, const char*, va_list);

/**
 * @param buf 
 * @param mode 
 * @return int 
 */
int setvbuf(FILE*, char* buf, int mode, size_t);

/**
 * @param buf 
 */
void setbuf(FILE*, char* buf);

/// @brief: setlinebuf
void setlinebuf(FILE*);

/**
 * @param oldpath 
 * @param newpath 
 * @return int 
 */
int rename(const char* oldpath, const char* newpath);

/**
 * @return FILE* 
 */
FILE* tmpfile();

/**
 * @return char* 
 */
char* tmpnam(char*);

/**
 * @param command 
 * @param type 
 * @return FILE* 
 */
FILE* popen(const char* command, const char* type);

/**
 * @return int 
 */
int pclose(FILE*);

__END_DECLS
