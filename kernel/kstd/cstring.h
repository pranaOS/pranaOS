/**
 * @file cstring.h
 * @author Krisna Pranav
 * @brief cstring
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav 
 * 
 */

#pragma once

#include "types.h"

#ifdef PRANAOS_KERNEL

/**
 * @brief stcat
 * 
 * @param dest 
 * @param src 
 * @return char* 
 */
char *strcat(char *dest, const char *src);

/**
 * @brief strcmp
 * 
 * @param str1 
 * @param str2 
 * @return true 
 * @return false 
 */
bool strcmp(const char *str1, const char *str2);

/**
 * @brief memset
 * 
 */
extern "C" void *memset(void *dest, int val, size_t count);

/**
 * @brief memcpy
 * 
 */
extern "C" void *memcpy(void *dest, const void *src, size_t count);

/**
 * @brief memcpy_uint32
 * 
 * @param d 
 * @param s 
 * @param n 
 * @return void* 
 */
void* memcpy_uint32(uint32_t* d, uint32_t* s, size_t n);

/**
 * @brief strlen
 * 
 * @param str 
 * @return int 
 */
int strlen(const char *str);

/**
 * @brief substr
 * 
 * @param i 
 * @param src 
 * @param dest 
 */
void substr(int i, char *src, char *dest);

/**
 * @brief substri
 * 
 * @param i 
 * @param src 
 * @param dest 
 */
void substri(int i, char *src, char *dest);

/**
 * @brief substrr
 * 
 * @param s 
 * @param e 
 * @param src 
 * @param dest 
 */
void substrr(int s, int e, char *src, char *dest);

/**
 * @brief strcpy
 * 
 * @param dest 
 * @param src 
 */
void strcpy(char *dest, const char *src);

#endif