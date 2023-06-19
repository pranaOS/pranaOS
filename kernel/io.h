/**
 * @file io.h
 * @author Krisna Pranav
 * @brief IO
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kstd/unix_types.h"

namespace IO {
    void wait();

    uint8_t inb(uint16_t port);

    inline void wait(size_t us) {
        while(us--) {
            inb(0x80)
        }
    }
};