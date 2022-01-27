// reference: http://www.jbox.dk/sanos/source/lib/stdlib.c.html

#pragma once

#include <stdint.h>
#include <stddef.h>

void aligned_alloc(size_t alignment, size_t size);

#ifndef _pranaOS_
void* kamalloc(size_t size, size_t align);
uint32_t memory_usage();

#define malloc kmalloc
#define free kfree

__attribute__((__noreturn__))
void exit(int status);
int system(const char* command);
#endif

void* malloc(size_t size);
void* calloc(size_t nmemb, size_t size);
void* zalloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

__attribute__((__noreturn__))
void abort();

char* itoa(int value, char* str, int base);
int atoi(const char* s);
int abs(int n);
int rand();

long int strtol(const char* nptr, char** endptr, int base);
double strtod(const char* nptr, char** endptr);

void srand(unsigned int seed);