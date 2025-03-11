/**
 * @file verify.h
 * @author Krisna Pranav
 * @brief verify
 * @version 6.0
 * @date 2023-08-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

struct crypt_data {
    int initialized;
    char result[65];
}; // struct crypt_data

/**
 * @param key 
 * @param salt 
 * @return char* 
 */
char* crypt(char const* key, char const* salt);

/**
 * @param key 
 * @param salt 
 * @param data 
 * @return char* 
 */
char* crypt_r(char const* key, char const* salt, struct crypt_data* data);

__END_DECLS
