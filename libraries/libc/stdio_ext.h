/**
 * @file stdio_ext.h
 * @author Krisna Pranav
 * @brief stdio extension
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdio.h>

__BEGIN_DECLS

size_t __fpending(FILE*);
int __freading(FILE*);
int __fwriting(FILE*);
void __fpurge(FILE*);

size_t __freadahead(FILE*);
char const* __freadptr(FILE*, size_t*);
void __freadptrinc(FILE*, size_t);
void __fseterr(FILE*);

__END_DECLS
