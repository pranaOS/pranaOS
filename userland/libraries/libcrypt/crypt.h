/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

struct crypt_data {
    int initialized;
    char result[65];
};

char* crypt(const char* key, const char* salt);
char* crypt_r(const char* key, const char* salt, struct crypt_data* data);

__END_DECLS
