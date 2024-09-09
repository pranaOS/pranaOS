/**
 * @file un.h
 * @author Krisna Pranav
 * @brief un
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UNIX_PATH_MAX 108

struct sockaddr_un 
{
    uint16_t sun_family;
    char sun_path[UNIX_PATH_MAX];
}; // struct sockaddr_un

#ifdef __cplusplus
}
#endif
