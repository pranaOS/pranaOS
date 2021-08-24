#ifndef _LIBC_STRING_H
#define _LIBC_STRING_H

// includes
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

void* memset(void* dest, int fill, size_t nbytes);

void* memmove(void* dest, const void* __restrict src, size_t nbytes);

void* memcpy(void* __restrict dest, const void* __restrict src, size_t nbytes);

void* memccpy(void* dest, const void* __restrict src, int stop, size_t nbytes);

int memcmp(const void* src1, const void* src2, size_t nbytes);

size_t strlen(const char* str);

char* strcpy(char* dest, const char* src);

char* strncpy(char* dest, const char* src, size_t nbytes);

__END_DECLS

#endif 