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


#ifndef __KERNEL__
FILE* fopen(const char* path, const char* mode);
int fclose(FILE* stream);
#endif