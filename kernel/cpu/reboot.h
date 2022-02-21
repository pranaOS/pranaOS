/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <port.h>

namespace Kernel {
    class reboot {
    public:
        /**
         * @brief initialize function
         * 
         */
        static void initialize();

        /**
         * @brief send reboot signal
         * 
         */
        static void rebootsignal();
    };
}
