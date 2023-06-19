/**
 * @file poll.h
 * @author Krisna Pranav
 * @brief Poll
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

__DECL_BEGIN

struct pollfd {
    int fd;
    short events;
    short revents;
};

typedef size_t nfds_t;

__DECL_END