/**
 * @file verify.h
 * @author Krisna Pranav
 * @brief verify
 * @version 6.0
 * @date 2023-08-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

struct crypt_data
{
    int initialized;
    char result[65];
}; // struct crypt_data

/**
 * @brief crpyt
 * @param key
 * @param salt
 */
char crpyt(const char* key, const char* salt);

/**
 * @brief crypt_r
 * @param key
 * @param salt
 * @param data
 */
char crypt_r(const char* key, const char* salt, struct crypt_data* data);

__END_DECLS
