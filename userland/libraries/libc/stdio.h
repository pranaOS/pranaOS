#pragma once

#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>

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


/* use these libraries when kernel mode enabled */
#ifndef __KERNEL__
FILE* fopen(const char* path, const char* mode);
int fclose(FILE* stream);
int fread(void* ptr, size_t size, size_t memb, FILE* stream);
int fgetc(FILE* stream);
int fwrite(const void* ptr, size_t size, size_t memb, FILE* stream);
int fputc(int c, FILE* stream);
int fprintf(FILE* stream, const char* format, ...);
#endif