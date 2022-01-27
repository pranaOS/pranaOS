/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#ifndef BUS_KERNEL_HEADER
#define BUS_KERNEL_HEADER

#include "address.h"

namespace Kernel {
    class Bus {
      public:
        void busInstall(void);
        void enable_api(Kernel::Address);
        void disable_api(Kernel::Address);

        void initialize();

      private:
        Bus();
    };
}

#endif 