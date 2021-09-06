/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/* Set 'nbytes' to 'fill' starting from 'dest'. */
void* memset(void* dest, int fill, size_t nbytes);

/* Move 'nbytes' from 'src' to 'dest' */
void* memmove(void* dest, const void* __restrict src, size_t nbytes);

/* Copy 'nbytes' from 'src' to 'dest'. See the comment in the source file
   about optimization and restricting pointers. */
void* memcpy(void* __restrict dest, const void* __restrict src, size_t nbytes);

/* Copy 'nbytes' from 'src' to 'dest', stopping if the current byte matches
   'stop'. Note that the stop byte also gets copied over. */
void* memccpy(void* dest, const void* __restrict src, int stop, size_t nbytes);

/* Compare 'nbytes' from 'src1' and 'src2'. Return 0 if all the bytes match,
   otherwise return the difference. */
int memcmp(const void* src1, const void* src2, size_t nbytes);

/* Calculate the string length starting from 'str'. */
size_t strlen(const char* str);

/* Copy 'src' into 'dest' until it finds a null byte in the source string.
   Note that this is dangerous because it writes memory no matter the size
   the 'dest' buffer is. */
char* strcpy(char* dest, const char* src);
int strcmp(const char* a, const char* b);

/* Copy 'src' into 'dest' until it finds a null byte or reaches the 'nbytes'
   limit provided by the user. This is the recommended way of copying strings,
   instead of using regular strcpy. Note that this will fill the 'dest' buffer
   with null bytes if the amount of copied bytes is lower than 'nbytes'. */
char* strncpy(char* dest, const char* src, size_t nbytes);

__END_DECLS