/**
 * @file limits.h
 * @author Krisna Pranav
 * @brief limits
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <bits/stdint.h>

#ifndef PAGE_SIZE
#   define PAGE_SIZE 4096
#endif

#define NAME_MAX 255

#define PIPE_BUF 4096

#define INT_MAX INT32_MAX
#define INT_MIN INT32_MIN