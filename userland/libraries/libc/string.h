/**
 * @file string.h
 * @author Krisna Pranav
 * @brief string
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#include <strings.h>

/**
 * @return size_t 
 */
size_t strlen(char const*);

/**
 * @param maxlen 
 * @return size_t 
 */
size_t strnlen(char const*, size_t maxlen);

/**
 * @return int 
 */
int strcmp(char const*, char const*);

/**
 * @return int 
 */
int strncmp(char const*, char const*, size_t);

/**
 * @return int 
 */
int memcmp(void const*, void const*, size_t);

/**
 * @return int 
 */
int timingsafe_memcmp(void const*, void const*, size_t);

/**
 * @return void* 
 */
void* memcpy(void*, void const*, size_t);

/**
 * @return void* 
 */
void* memmove(void*, void const*, size_t);

/**
 * @param c 
 * @return void* 
 */
void* memchr(void const*, int c, size_t);

/**
 * @param haystack 
 * @param needle 
 * @return void const* 
 */
void const* memmem(void const* haystack, size_t, void const* needle, size_t);

/**
 * @return void* 
 */
void* memset(void*, int, size_t);

void explicit_bzero(void*, size_t) __attribute__((nonnull(1)));

__attribute__((malloc)) char* strdup(char const*);
__attribute__((malloc)) char* strndup(char const*, size_t);

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcpy(char* dest, char const* src);

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strncpy(char* dest, char const* src, size_t);

__attribute__((warn_unused_result)) size_t strlcpy(char* dest, char const* src, size_t);

/**
 * @param c 
 * @return char* 
 */
char* strchr(char const*, int c);

/**
 * @param c 
 * @return char* 
 */
char* strchrnul(char const*, int c);

/**
 * @param haystack 
 * @param needle 
 * @return char* 
 */
char* strstr(char const* haystack, char const* needle);

/**
 * @param c 
 * @return char* 
 */
char* strrchr(char const*, int c);

/**
 * @param str 
 * @param ch 
 * @return char* 
 */
char* index(char const* str, int ch);

/**
 * @param str 
 * @param ch 
 * @return char* 
 */
char* rindex(char const* str, int ch);

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strcat(char* dest, char const* src);

/**
 * @param dest 
 * @param src 
 * @return char* 
 */
char* strncat(char* dest, char const* src, size_t);

/**
 * @param accept 
 * @return size_t 
 */
size_t strspn(char const*, char const* accept);

/**
 * @param reject 
 * @return size_t 
 */
size_t strcspn(char const*, char const* reject);

/**
 * @return int 
 */
int strerror_r(int, char*, size_t);

/**
 * @param errnum 
 * @return char* 
 */
char* strerror(int errnum);

/**
 * @param signum 
 * @return char* 
 */
char* strsignal(int signum);

/**
 * @param accept 
 * @return char* 
 */
char* strpbrk(char const*, char const* accept);

/**
 * @param str 
 * @param delim 
 * @param saved_str 
 * @return char* 
 */
char* strtok_r(char* str, char const* delim, char** saved_str);

/**
 * @param str 
 * @param delim 
 * @return char* 
 */
char* strtok(char* str, char const* delim);

/**
 * @param s1 
 * @param s2 
 * @return int 
 */
int strcoll(char const* s1, char const* s2);

/**
 * @param dest 
 * @param src 
 * @param n 
 * @return size_t 
 */
size_t strxfrm(char* dest, char const* src, size_t n);

/**
 * @param str 
 * @param delim 
 * @return char* 
 */
char* strsep(char** str, char const* delim);

__END_DECLS
