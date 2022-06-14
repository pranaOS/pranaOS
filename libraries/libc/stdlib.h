#pragma once

#include <cheader.h>
#include <stddef.h>

_Begin_CHeader

extern void exit(int status);
extern char * getenv(const char *name);

extern void *malloc(size_t size);
extern void free(void *ptr);
extern void *calloc(size_t nmemb, size_t size);
extern void *realloc(void *ptr, size_t size);

extern void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));

extern int system(const char * command);

extern int abs(int j);

extern int putenv(char * name);
extern int setenv(const char *name, const char *value, int overwrite);
extern int unsetenv(const char * str);

extern void abort(void);

#define NULL 0

_End_CHeader