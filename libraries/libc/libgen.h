/**
 * @file libgen.h
 * @author Krisna Pranav
 * @brief libgen
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @param path 
 * @return char* 
 */
char* dirname(char* path);

/**
 * @param path 
 * @return char* 
 */
char* basename(char* path);

__END_DECLS