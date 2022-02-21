/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <port.h>

namespace Kernel {
    class powerOff {
    public:
        /**
         * @brief initialize poweroff function
         * 
         */
        static void initialize();

        /**
         * @brief send poweroff 
         * 
         */
        static void poweroff();
    };
}
