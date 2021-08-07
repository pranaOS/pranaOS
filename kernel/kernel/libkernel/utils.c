/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/libkernel.h>

int stoi(void* strv, int len)
{
    char* str = (char*)strv;
    if (len > 9) {
        return 0;
    }
    int res = 0;
    int mult = 1;
    char* end = str + len - 1;
    while (end >= str) {
        res += (*end - '0') * mult;
        mult *= 10;
        end--;
    }
    return res;
}

bool str_validate_len(const char* s, uint32_t len)
{
    for (int i = 0; i < len + 1; i++) {
        if (s[i] == 0) {
            return true;
        }
    }
    return false;
}

uint32_t ptrarr_len(const char** s)
{
    uint32_t len = 0;
    while (s[len] != 0) {
        ++len;
    }
    return len;
}

bool ptrarr_validate_len(const char** s, uint32_t len)
{
    for (int i = 0; i < len + 1; i++) {
        if (s[i] == 0) {
            return true;
        }
    }
    return false;
}