#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#define EOF -1

typedef struct {
    int32_t fd;
    char* name;
} FILE;

extern FILE* stdout;
extern FILE* stderr;

int putchar(int);
int puts(const char*);

int printf(const char* format, ...);
int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t size, const char* format, ...);
int vsprintf(char* str, const char* format, va_list ap);
int vsnprintf(char* str, size_t size, const char* format, va_list ap);


/**
 * @brief use these functions when pranaOS kernel mode enabled
 * 
 */
#ifndef _pranaOS_
FILE* fopen(const char* path, const char* mode);
int fclose(FILE* stream);
int fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
int fgetc(FILE* stream);
int fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int fputc(int c, FILE* stream);
int fprintf(FILE* stream, const char* format, ...);
int vfprintf(FILE* stream, const char* format, va_list ap);
int fseek(FILE* stream, long offset, int whence);
long ftell(FILE* stream);
int fflush(FILE* stream);
int rename(const char* old, const char* new);
int remove(const char* pathname);
#endif