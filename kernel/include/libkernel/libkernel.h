/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_LIBKERNEL_H
#define _KERNEL_LIBKERNEL_LIBKERNEL_H

// includes
#include <libkernel/kassert.h>
#include <libkernel/mem.h>
#include <libkernel/types.h>


int stoi(void* str, int len);
void htos(uint32_t hex, char str[]);
void dtos(uint32_t dec, char str[]);
void reverse(char s[]);
uint32_t strlen(const char* s);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, uint32_t num);
bool str_validate_len(const char* c, uint32_t len);

uint32_t ptrarr_len(const char** s);
bool ptrarr_validate_len(const char** s, uint32_t len);

static inline int imax(int a, int b)
{
    return a > b ? a : b;
}
static inline int imin(int a, int b)
{
    return a < b ? a : b;
}
static inline uint32_t u32max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}
static inline uint32_t u32min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

#endif