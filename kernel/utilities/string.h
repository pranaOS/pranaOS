/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <ak/cdefs.h>
#include <ak/types.h>

#define op_t unsigned long int
#define OPSIZ (sizeof(op_t))

/**
 * @brief itoa
 * 
 * @param i 
 * @param base 
 * @param buf 
 */
void itoa(long long i, unsigned base, char *buf);
void itoa_s(long long i, unsigned base, char *buf);

/**
 * @brief atoi
 * 
 * @param buf 
 * @return int 
 */
int atoi(const char *buf);

/**
 * @brief strcasecmp
 * 
 * @param s1 
 * @param s2 
 * @return int 
 */
int strcasecmp(const char *s1, const char *s2);

/**
 * @brief strcat
 * 
 * @param dest 
 * @param src 
 * @return char* 
 */
char *strcat(char *dest, const char *src);

/**
 * @brief strchr + nul
 * 
 * @param s 
 * @param c_in 
 * @return char* 
 */
char *strchr(const char *s, int c_in);
char *strchrnul(const char *s, int c_in);

/**
 * @brief strcmp
 * 
 * @param p1 
 * @param p2 
 * @return int 
 */
int strcmp(const char *p1, const char *p2);

/**
 * @brief strcpy
 * 
 * @param dest 
 * @param src 
 * @return char* 
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief strcspn
 * 
 * @param str 
 * @param reject 
 * @return size_t 
 */
size_t strcspn(const char *str, const char *reject);

/**
 * @brief strdup
 * 
 * @param s 
 * @return char* 
 */
char *strdup(const char *s);

/**
 * @brief strlen
 * 
 * @param str 
 * @return size_t 
 */
size_t strlen(const char *str);

/**
 * @brief strncasecmp
 * 
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int strncasecmp(const char *s1, const char *s2, int n);

/**
 * @brief strncat
 * 
 * @param s1 
 * @param s2 
 * @param n 
 * @return char* 
 */
char *strncat(char *s1, const char *s2, size_t n);

/**
 * @brief strncmp
 * 
 * @param s1 
 * @param s2 
 * @param n 
 * @return int 
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief strncpy
 * 
 * @param s1 
 * @param s2 
 * @param n 
 * @return char* 
 */
char *strncpy(char *s1, const char *s2, size_t n);

/**
 * @brief strnlen
 * 
 * @param str 
 * @param maxlen 
 * @return size_t 
 */
size_t strnlen(const char *str, size_t maxlen);

/**
 * @brief strpbrk
 * 
 * @param s 
 * @param accept 
 * @return char* 
 */
char *strpbrk(const char *s, const char *accept);

/**
 * @brief strrchr
 * 
 * @param s 
 * @param c 
 * @return char* 
 */
char *strrchr(const char *s, int c);

/**
 * @brief strsep
 * 
 * @param stringp 
 * @param delim 
 * @return char* 
 */
char *strsep(char **stringp, const char *delim);

/**
 * @brief strim
 * 
 * @param s 
 * @return char* 
 */
char *strim(char *s);

/**
 * @brief strr str
 * 
 * @param string 
 * @param find 
 * @return char* 
 */
char *strrstr(char *string, char *find);

/**
 * @brief str replace
 * 
 * @param s 
 * @param old 
 * @param new 
 * @return char* 
 */
char *strreplace(char *s, char old, char new);

/**
 * @brief str iof
 * 
 * @param s1 
 * @param s2 
 * @return int32_t 
 */
int32_t striof(const char *s1, const char *s2);

/**
 * @brief strliof
 * 
 * @param s1 
 * @param s2 
 * @return int32_t 
 */
int32_t strliof(const char *s1, const char *s2);

/**
 * @brief strlsplat
 * 
 * @param s1 
 * @param pos 
 * @param sf 
 * @param sl 
 * @return int32_t 
 */
int32_t strlsplat(const char *s1, int32_t pos, char **sf, char **sl);

/**
 * @brief skipspaces
 * 
 * @param str 
 * @return char* 
 */
char *skip_spaces(const char *str);

/**
 * @brief memcpy
 * 
 * @param dest 
 * @param src 
 * @param len 
 * @return void* 
 */
void *memcpy(void *dest, const void *src, size_t len);

/**
 * @brief memset
 * 
 * @param dest 
 * @param val 
 * @param len 
 * @return void* 
 */
void *memset(void *dest, char val, size_t len);

/**
 * @brief memcmp
 * 
 * @param vl 
 * @param vr 
 * @param n 
 * @return int 
 */
int memcmp(const void *vl, const void *vr, size_t n);

/**
 * @brief count array of pointer
 * 
 * @param arr 
 * @return int 
 */
int count_array_of_pointers(void *arr);