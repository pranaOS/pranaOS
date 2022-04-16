/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>

namespace pranaOS {
    struct VM86Args {
        uint16_t AX;
        uint16_t BX;
        uint16_t CX;
        uint16_t DX;
        uint16_t DI;
            
    } __attribute__((packed));
}
