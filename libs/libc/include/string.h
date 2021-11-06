#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"

extern void memcpy(void *dest, void *src, uint32_t len);
extern void memset(void *dest, uint8_t val, uint32_t len);
extern void bzero(void *dest, uint32_t len);

extern int8_t strcmp(const char *src, const char *dest);
extern char * strcpy(char *dest, const char *src);
extern char * strncpy(char *dest, const char *src, uint32_t len);

extern char *strcat(char *dest, const char *src);

extern size_t strlen(const char *src);
extern void   backspace(char *src);
extern void   append(char *src, char dest);

#ifdef __cplusplus
}
#endif