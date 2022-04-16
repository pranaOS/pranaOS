/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libc/list/list.h>
#include <ak/types.h>

namespace Kernel {
    class interruptHandler {
    public:
        interruptHandler(uint8_t intNumber);
        virtual uint32_t handleInterrupt(uint32_t esp);
    };

    class interruptManager {
    public:
        static void initialize();
        static uint32_t handleInterrupt(uint8_t interrupt, uint32_t esp);

        static void addHandler(interruptHandler* handler,uint8_t interrupt);
        static void removeHandler(interruptHandler* handler, uint8_t interrupt);
    
    private:
        static LibC::list<interruptHandler*>* interruptCallbacks[256];
    };
}
