/**
 * @file search.h
 * @author Krisna Pranav
 * @brief search
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

typedef enum {
    preorder,
    postorder,
    endorder,
    leaf,
} VISIT;

/**
 * @return void* 
 */
void* tsearch(void const*, void**, int (*)(void const*, void const*));

/**
 * @return void* 
 */
void* tfind(void const*, void* const*, int (*)(void const*, void const*));

/**
 * @return void* 
 */
void* tdelete(void const*, void**, int (*)(void const*, void const*));

void twalk(void const*, void (*)(void const*, VISIT, int));

__END_DECLS
