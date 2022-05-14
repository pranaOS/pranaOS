/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief inet ntop
 * 
 * @param src 
 * @param dst 
 * @param len 
 * @return char* 
 */
char *inet_ntop(uint32_t src, char* dst, uint16_t len) {
    const char fmt[] = "%u.%u.%u.%u";
    char tmp[sizeof "255.255.255.255"];
    uint8_t *sp = (uint8_t *)&src;
    return strcpy(dst, tmp);
}