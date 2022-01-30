/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include <ak/list.h>
#include <ak/types.h>

namespace Kernel {
    class interruptHandler {
    public:
        interruptHandler(ak::uint8_t intNumber);
        virtual ak::uint32_t handleInterrupt(ak::uint32_t esp);
    };

    class interruptManager {
    public:
        static void initialize();
        static ak::uint32_t handleInterrupt(ak::uint8_t interrupt, ak::uint32_t esp);

        static void addHandler(interruptHandler* handler, ak::uint8_t interrupt);
        static void removeHandler(interruptHandler* handler, ak::uint8_t interrupt);
    
    private:
        static List<interruptHandler*>* interruptCallbacks[256];
    };
}
