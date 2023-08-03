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
char* fgets(char* buffer, int size, FILE*);
int fputc(int ch, FILE*);
int fileno(FILE*);
int fgetc(FILE*);
int getc(FILE*);
int getc_unlocked(FILE* stream);
int getchar();
ssize_t getdelim(char**, size_t*, int, FILE*);
ssize_t getline(char**, size_t*, FILE*);
int ungetc(int c, FILE*);
int remove(const char* pathname);
FILE* fdopen(int fd, const char* mode);
FILE* fopen(const char* pathname, const char* mode);
FILE* freopen(const char* pathname, const char* mode, FILE*);
void flockfile(FILE* filehandle);
void funlockfile(FILE* filehandle);
int fclose(FILE*);
void rewind(FILE*);
void clearerr(FILE*);
int ferror(FILE*);
int feof(FILE*);
int fflush(FILE*);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE*);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE*);
int vprintf(const char* fmt, va_list);
int vfprintf(FILE*, const char* fmt, va_list);
int vsprintf(char* buffer, const char* fmt, va_list);
int vsnprintf(char* buffer, size_t, const char* fmt, va_list);
int fprintf(FILE*, const char* fmt, ...);
int printf(const char* fmt, ...);
int dbgprintf(const char* fmt, ...);
void dbgputch(char);
int dbgputstr(const char*, ssize_t);
int sprintf(char* buffer, const char* fmt, ...);
int snprintf(char* buffer, size_t, const char* fmt, ...);
int putchar(int ch);
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
