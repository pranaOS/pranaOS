/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include <stddef.h>

#ifndef __cplusplus 
extern "C" {
#endif 

/**
 * @brief string length functions
 * 
 * @param str 
 * @return size_t 
 */
size_t strlen(const char* str);
size_t strnlen(const char* s, size_t maxlen);

/**
 * @brief comparing string 1 and string 2 function
 * 
 * @param a 
 * @param b 
 * @param n 
 * @return int 
 */
int strncasecmp(const char* a, const char* b, size_t n);
int strcasecmp(const char* a, const char* b);
int strncmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
char* strdup(const char* src);
char* strchr(const char* c, int d);
char* strchrnil(const char* c, int d);


#ifndef __cplusplus
}

#endif 