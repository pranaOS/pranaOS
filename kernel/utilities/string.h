/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <ak/cdefs.h>
#include <ak/types.h>

#define op_t unsigned long int
#define OPSIZ (sizeof(op_t))

/**
 * @brief iota
 * 
 * @param i 
 * @param base 
 * @param buf 
 */
void itoa(long long i, unsigned base, char *buf);
void itoa_s(long long i, unsigned base, char *buf);

/**
 * @brief atio
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
 * @brief strchr, strchrnull
 * 
 * @param s 
 * @param c_in 
 * @return char* 
 */
char *strchr(const char *s, int c_in);
char *strchrnull(const char *s, int c_in);

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
 * @brief strcpn
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